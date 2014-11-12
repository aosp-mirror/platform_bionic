
import java.io.*;
import java.util.*;

// usage: java ZoneCompiler <setup file> <data directory> <output directory> <tzdata version>
//
// Compile a set of tzfile-formatted files into a single file containing an index.
//
// The compilation is controlled by a setup file, which is provided as a
// command-line argument.  The setup file has the form:
//
// Link <toName> <fromName>
// ...
// <zone filename>
// ...
//
// Note that the links must be declared prior to the zone names.
// A zone name is a filename relative to the source directory such as
// 'GMT', 'Africa/Dakar', or 'America/Argentina/Jujuy'.
//
// Use the 'zic' command-line tool to convert from flat files
// (such as 'africa' or 'northamerica') to a directory
// hierarchy suitable for this tool (containing files such as 'data/Africa/Abidjan').
//

public class ZoneCompactor {
  // Maximum number of characters in a zone name, including '\0' terminator.
  private static final int MAXNAME = 40;

  // Zone name synonyms.
  private Map<String,String> links = new HashMap<String,String>();

  // File offsets by zone name.
  private Map<String,Integer> offsets = new HashMap<String,Integer>();

  // File lengths by zone name.
  private Map<String,Integer> lengths = new HashMap<String,Integer>();

  // Concatenate the contents of 'inFile' onto 'out'.
  private static void copyFile(File inFile, OutputStream out) throws Exception {
    byte[] ret = new byte[0];

    InputStream in = new FileInputStream(inFile);
    byte[] buf = new byte[8192];
    while (true) {
      int nbytes = in.read(buf);
      if (nbytes == -1) {
        break;
      }
      out.write(buf, 0, nbytes);

      byte[] nret = new byte[ret.length + nbytes];
      System.arraycopy(ret, 0, nret, 0, ret.length);
      System.arraycopy(buf, 0, nret, ret.length, nbytes);
      ret = nret;
    }
    out.flush();
  }

  public ZoneCompactor(String setupFile, String dataDirectory, String zoneTabFile, String outputDirectory, String version) throws Exception {
    // Read the setup file and concatenate all the data.
    ByteArrayOutputStream allData = new ByteArrayOutputStream();
    BufferedReader reader = new BufferedReader(new FileReader(setupFile));
    String s;
    int offset = 0;
    while ((s = reader.readLine()) != null) {
      s = s.trim();
      if (s.startsWith("Link")) {
        StringTokenizer st = new StringTokenizer(s);
        st.nextToken();
        String to = st.nextToken();
        String from = st.nextToken();
        links.put(from, to);
      } else {
        String link = links.get(s);
        if (link == null) {
          File sourceFile = new File(dataDirectory, s);
          long length = sourceFile.length();
          offsets.put(s, offset);
          lengths.put(s, (int) length);

          offset += length;
          copyFile(sourceFile, allData);
        }
      }
    }
    reader.close();

    // Fill in fields for links.
    Iterator<String> it = links.keySet().iterator();
    while (it.hasNext()) {
      String from = it.next();
      String to = links.get(from);

      offsets.put(from, offsets.get(to));
      lengths.put(from, lengths.get(to));
    }

    // Create/truncate the destination file.
    RandomAccessFile f = new RandomAccessFile(new File(outputDirectory, "tzdata"), "rw");
    f.setLength(0);

    // Write the header.

    // byte[12] tzdata_version -- 'tzdata2012f\0'
    // int index_offset -- so we can slip in extra header fields in a backwards-compatible way
    // int data_offset
    // int zonetab_offset

    // tzdata_version
    f.write(toAscii(new byte[12], version));

    // Write dummy values for the three offsets, and remember where we need to seek back to later
    // when we have the real values.
    int index_offset_offset = (int) f.getFilePointer();
    f.writeInt(0);
    int data_offset_offset = (int) f.getFilePointer();
    f.writeInt(0);
    int zonetab_offset_offset = (int) f.getFilePointer();
    f.writeInt(0);

    int index_offset = (int) f.getFilePointer();

    // Write the index.
    ArrayList<String> sortedOlsonIds = new ArrayList<String>();
    sortedOlsonIds.addAll(offsets.keySet());
    Collections.sort(sortedOlsonIds);
    it = sortedOlsonIds.iterator();
    while (it.hasNext()) {
      String zoneName = it.next();
      if (zoneName.length() >= MAXNAME) {
        throw new RuntimeException("zone filename too long: " + zoneName.length());
      }

      // Follow the chain of links to work out where the real data for this zone lives.
      String actualZoneName = zoneName;
      while (links.get(actualZoneName) != null) {
        actualZoneName = links.get(actualZoneName);
      }

      f.write(toAscii(new byte[MAXNAME], zoneName));
      f.writeInt(offsets.get(actualZoneName));
      f.writeInt(lengths.get(actualZoneName));
      f.writeInt(0); // Used to be raw GMT offset. No longer used.
    }

    int data_offset = (int) f.getFilePointer();

    // Write the data.
    f.write(allData.toByteArray());

    int zonetab_offset = (int) f.getFilePointer();

    // Copy the zone.tab.
    reader = new BufferedReader(new FileReader(zoneTabFile));
    while ((s = reader.readLine()) != null) {
      if (!s.startsWith("#")) {
        f.writeBytes(s);
        f.write('\n');
      }
    }
    reader.close();

    // Go back and fix up the offsets in the header.
    f.seek(index_offset_offset);
    f.writeInt(index_offset);
    f.seek(data_offset_offset);
    f.writeInt(data_offset);
    f.seek(zonetab_offset_offset);
    f.writeInt(zonetab_offset);

    f.close();
  }

  private static byte[] toAscii(byte[] dst, String src) {
    for (int i = 0; i < src.length(); ++i) {
      if (src.charAt(i) > '~') {
        throw new RuntimeException("non-ASCII string: " + src);
      }
      dst[i] = (byte) src.charAt(i);
    }
    return dst;
  }

  public static void main(String[] args) throws Exception {
    if (args.length != 5) {
      System.err.println("usage: java ZoneCompactor <setup file> <data directory> <zone.tab file> <output directory> <tzdata version>");
      System.exit(0);
    }
    new ZoneCompactor(args[0], args[1], args[2], args[3], args[4]);
  }
}
