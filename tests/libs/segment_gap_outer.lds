SECTIONS {
  # This starts off fairly normal: rodata, text, dynamic, data, bss with
  # appropriate alignment between them.
  . = SIZEOF_HEADERS;
  .rodata : {*(.rodata .rodata.*)}
  . = ALIGN(0x1000);
  .text : {*(.text .text.*)}
  . = ALIGN(0x1000);
  .dynamic : {*(.dynamic)}
  . = ALIGN(0x1000);
  .data : {*(.data .data.*)}
  .bss : {*(.bss .bss.*)}

  # Now create the gap. We need a text segment first to prevent the linker from
  # merging .bss with .custom_bss.
  . = ALIGN(0x1000);
  .custom_text : {
    *(.custom_text);
  }

  # Place custom_bss at the end of the gap.
  . = 0x1000000;
  .custom_bss : {
    *(.custom_bss);
  }
}
