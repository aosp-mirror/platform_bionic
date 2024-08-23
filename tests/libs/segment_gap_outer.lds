SECTIONS {
  # This starts off fairly normal: rodata, text, dynamic, data, bss with
  # appropriate alignment between them.
  . = SIZEOF_HEADERS;
  .rodata : {*(.rodata .rodata.*)}
  . = ALIGN(CONSTANT (MAXPAGESIZE));
  .text : {*(.text .text.*)}
  . = ALIGN(CONSTANT (MAXPAGESIZE));
  .dynamic : {*(.dynamic)}
  . = ALIGN(CONSTANT (MAXPAGESIZE));
  .data : {*(.data .data.*)}
  .bss : {*(.bss .bss.*)}

  # Now create the gap. We need a text segment first to prevent the linker from
  # merging .bss with .custom_bss.
  . = ALIGN(CONSTANT (MAXPAGESIZE));
  .custom_text : {
    *(.custom_text);
  }

  # Place custom_bss at the end of the gap.
  . = 0x1000000;
  .custom_bss : {
    *(.custom_bss);
  }
}
