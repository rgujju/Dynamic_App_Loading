 OUTPUT_FORMAT("elf32-littlearm")
_region_min_align = 32;
MEMORY
    {
    FLASH (rx) : ORIGIN = (0x0 + 0x0), LENGTH = (1024*1K - 0x0)
    SRAM (wx) : ORIGIN = 0x20000000, LENGTH = (256 * 1K)
    IDT_LIST (wx) : ORIGIN = (0x20000000 + (256 * 1K)), LENGTH = 2K
    }
ENTRY("__start")
SECTIONS
    {
 .rel.plt :
 {
 *(.rel.plt)
 PROVIDE_HIDDEN (__rel_iplt_start = .);
 *(.rel.iplt)
 PROVIDE_HIDDEN (__rel_iplt_end = .);
 }
 .rela.plt :
 {
 *(.rela.plt)
 PROVIDE_HIDDEN (__rela_iplt_start = .);
 *(.rela.iplt)
 PROVIDE_HIDDEN (__rela_iplt_end = .);
 }
 .rel.dyn :
 {
 *(.rel.*)
 }
 .rela.dyn :
 {
 *(.rela.*)
 }
    /DISCARD/ :
 {
 *(.plt)
 }
    /DISCARD/ :
 {
 *(.iplt)
 }
   
 _image_rom_start = (0x0 + 0x0);
    rom_start :
 {
. = 0x0;
. = ALIGN(4);
_vector_start = .;
KEEP(*(.exc_vector_table))
KEEP(*(".exc_vector_table.*"))
KEEP(*(.gnu.linkonce.irq_vector_table))
KEEP(*(.vectors))
_vector_end = .;
KEEP(*(.openocd_dbg))
KEEP(*(".openocd_dbg.*"))
 } > FLASH
    text :
 {
 _image_text_start = .;
 _kobject_text_area_start = .;
 *(".kobject_data.text*")
 _kobject_text_area_end = .;
 _kobject_text_area_used = _kobject_text_area_end - _kobject_text_area_start;
 PROVIDE(z_object_find = .);
 PROVIDE(z_object_wordlist_foreach = .);
 . = MAX(., _kobject_text_area_start + 256);
 ASSERT(
  256 >= _kobject_text_area_used,
"The configuration system has incorrectly set
'CONFIG_KOBJECT_TEXT_AREA' to
256, which is not big enough. You must
through Kconfig either disable 'CONFIG_USERSPACE', or set
'CONFIG_KOBJECT_TEXT_AREA' to a value larger than
256 ."
  );
 *(.text)
 *(".text.*")
 *(.gnu.linkonce.t.*)
 *(.glue_7t) *(.glue_7) *(.vfp11_veneer) *(.v4_bx)
 } > FLASH
 _image_text_end = .;
 .ARM.exidx :
 {
 __exidx_start = .;
 *(.ARM.exidx* gnu.linkonce.armexidx.*)
 __exidx_end = .;
 } > FLASH
 _image_rodata_start = .;
 initlevel :
 {
  __init_start = .; __init_PRE_KERNEL_1_start = .; KEEP(*(SORT(.init_PRE_KERNEL_1[0-9]))); KEEP(*(SORT(.init_PRE_KERNEL_1[1-9][0-9]))); __init_PRE_KERNEL_2_start = .; KEEP(*(SORT(.init_PRE_KERNEL_2[0-9]))); KEEP(*(SORT(.init_PRE_KERNEL_2[1-9][0-9]))); __init_POST_KERNEL_start = .; KEEP(*(SORT(.init_POST_KERNEL[0-9]))); KEEP(*(SORT(.init_POST_KERNEL[1-9][0-9]))); __init_APPLICATION_start = .; KEEP(*(SORT(.init_APPLICATION[0-9]))); KEEP(*(SORT(.init_APPLICATION[1-9][0-9]))); __init_SMP_start = .; KEEP(*(SORT(.init_SMP[0-9]))); KEEP(*(SORT(.init_SMP[1-9][0-9]))); __init_end = .;
 } > FLASH
 sw_isr_table :
 {
  . = ALIGN(0);
  *(.gnu.linkonce.sw_isr_table)
 } > FLASH
 initlevel_error :
 {
  KEEP(*(SORT(.init_[_A-Z0-9]*)))
 }
 ASSERT(SIZEOF(initlevel_error) == 0, "Undefined initialization levels used.")
 object_access :
 {
  _z_object_assignment_list_start = .;
  KEEP(*("._z_object_assignment.*"))
  _z_object_assignment_list_end = .;
 } > FLASH
 app_shmem_regions :
 {
  __app_shmem_regions_start = .;
  KEEP(*(SORT(".app_regions.*")));
  __app_shmem_regions_end = .;
 } > FLASH
 net_l2 :
 {
  __net_l2_start = .;
  *(".net_l2.init")
  KEEP(*(SORT(".net_l2.init*")))
  __net_l2_end = .;
 } > FLASH
 _bt_channels_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _bt_l2cap_fixed_chan_list_start = .;
  KEEP(*(SORT("._bt_l2cap_fixed_chan.static.*")))
  _bt_l2cap_fixed_chan_list_end = .;
 } > FLASH
 _bt_services_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _bt_gatt_service_static_list_start = .;
  KEEP(*(SORT("._bt_gatt_service_static.static.*")))
  _bt_gatt_service_static_list_end = .;
 } > FLASH
 log_const_sections : ALIGN_WITH_INPUT
 {
  __log_const_start = .;
  KEEP(*(SORT(.log_const_*)));
  __log_const_end = .;
 } > FLASH
 log_backends_sections : ALIGN_WITH_INPUT
 {
  __log_backends_start = .;
  KEEP(*("._log_backend.*"));
  __log_backends_end = .;
 } > FLASH
 shell_sections : ALIGN_WITH_INPUT
 {
  _shell_list_start = .;
  KEEP(*(SORT(._shell.static.*)));
  _shell_list_end = .;
 } > FLASH
 shell_root_cmds_sections : ALIGN_WITH_INPUT
 {
  __shell_root_cmds_start = .;
  KEEP(*(SORT(.shell_root_cmd_*)));
  __shell_root_cmds_end = .;
 } > FLASH
 font_entry_sections : ALIGN_WITH_INPUT
 {
  __font_entry_start = .;
  KEEP(*(SORT("._cfb_font.*")))
  __font_entry_end = .;
 } > FLASH
 tracing_backends_sections : ALIGN_WITH_INPUT
 {
  _tracing_backend_list_start = .;
  KEEP(*("._tracing_backend.*"));
  _tracing_backend_list_end = .;
 } > FLASH
    rodata :
 {
 *(.rodata)
 *(".rodata.*")
 *(.gnu.linkonce.r.*)
 *(".kobject_data.rodata*")
 . = ALIGN(4);
 } > FLASH
 _image_rodata_end = .;
 . = ALIGN(_region_min_align); . = ALIGN( 1 << LOG2CEIL(_image_rodata_end -_image_rom_start));
 _image_rom_end = .;
   
    /DISCARD/ : {
 *(.got.plt)
 *(.igot.plt)
 *(.got)
 *(.igot)
 }
   
 . = 0x20000000;
 . = ALIGN(_region_min_align);
 _image_ram_start = .;
.ramfunc : ALIGN_WITH_INPUT
{
 . = ALIGN(_region_min_align); . = ALIGN( 1 << LOG2CEIL(_ramfunc_ram_size));
 _ramfunc_ram_start = .;
 *(.ramfunc)
 *(".ramfunc.*")
 . = ALIGN(_region_min_align); . = ALIGN( 1 << LOG2CEIL(_ramfunc_ram_size));
 _ramfunc_ram_end = .;
} > SRAM AT> FLASH
_ramfunc_ram_size = _ramfunc_ram_end - _ramfunc_ram_start;
_ramfunc_rom_start = LOADADDR(.ramfunc);
 app_smem :
 {
  . = ALIGN(_region_min_align);;
  _app_smem_start = .;
  . = ALIGN(_region_min_align);;
  _app_smem_end = .;
 } > SRAM AT> FLASH
 _app_smem_size = _app_smem_end - _app_smem_start;
 _app_smem_rom_start = LOADADDR(app_smem);
    bss (NOLOAD) : ALIGN_WITH_INPUT
 {
        . = ALIGN(4);
 __bss_start = .;
 __kernel_ram_start = .;
 *(.bss)
 *(".bss.*")
 *(COMMON)
 *(".kernel_bss.*")
 __bss_end = ALIGN(4);
 } > SRAM AT> SRAM
    noinit (NOLOAD) :
        {
        *(.noinit)
        *(".noinit.*")
 *(".kernel_noinit.*")
        } > SRAM
    datas : ALIGN_WITH_INPUT
 {
 __data_ram_start = .;
 *(.data)
 *(".data.*")
 *(".kernel.*")
 } > SRAM AT> FLASH
    __data_rom_start = LOADADDR(datas);
 devices : ALIGN_WITH_INPUT
 {
  __device_start = .; __device_PRE_KERNEL_1_start = .; KEEP(*(SORT(.device_PRE_KERNEL_1[0-9]))); KEEP(*(SORT(.device_PRE_KERNEL_1[1-9][0-9]))); __device_PRE_KERNEL_2_start = .; KEEP(*(SORT(.device_PRE_KERNEL_2[0-9]))); KEEP(*(SORT(.device_PRE_KERNEL_2[1-9][0-9]))); __device_POST_KERNEL_start = .; KEEP(*(SORT(.device_POST_KERNEL[0-9]))); KEEP(*(SORT(.device_POST_KERNEL[1-9][0-9]))); __device_APPLICATION_start = .; KEEP(*(SORT(.device_APPLICATION[0-9]))); KEEP(*(SORT(.device_APPLICATION[1-9][0-9]))); __device_SMP_start = .; KEEP(*(SORT(.device_SMP[0-9]))); KEEP(*(SORT(.device_SMP[1-9][0-9]))); __device_end = .;
 } > SRAM AT> FLASH
 initshell : ALIGN_WITH_INPUT
 {
  __shell_module_start = .; KEEP(*(".shell_module_*")); __shell_module_end = .; __shell_cmd_start = .; KEEP(*(".shell_cmd_*")); __shell_cmd_end = .;
 } > SRAM AT> FLASH
 log_dynamic_sections : ALIGN_WITH_INPUT
 {
  __log_dynamic_start = .;
  KEEP(*(SORT(.log_dynamic_*)));
  __log_dynamic_end = .;
 } > SRAM AT> FLASH
 _static_thread_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  __static_thread_data_list_start = .;
  KEEP(*(SORT(".__static_thread_data.static.*")))
  __static_thread_data_list_end = .;
 } > SRAM AT> FLASH
 _static_kernel_objects_begin = .;
 _k_timer_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_timer_list_start = .;
  KEEP(*("._k_timer.static.*"))
  _k_timer_list_end = .;
 } > SRAM AT> FLASH
 _k_mem_slab_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_mem_slab_list_start = .;
  KEEP(*("._k_mem_slab.static.*"))
  _k_mem_slab_list_end = .;
 } > SRAM AT> FLASH
 _k_mem_pool_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_mem_pool_list_start = .;
  KEEP(*("._k_mem_pool.static.*"))
  _k_mem_pool_list_end = .;
 } > SRAM AT> FLASH
 _k_heap_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_heap_list_start = .;
  KEEP(*("._k_heap.static.*"))
  _k_heap_list_end = .;
 } > SRAM AT> FLASH
 _k_sem_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_sem_list_start = .;
  KEEP(*("._k_sem.static.*"))
  KEEP(*("._sys_sem.static.*"))
  _k_sem_list_end = .;
 } > SRAM AT> FLASH
 _k_mutex_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_mutex_list_start = .;
  KEEP(*("._k_mutex.static.*"))
  _k_mutex_list_end = .;
 } > SRAM AT> FLASH
 _k_queue_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_queue_list_start = .;
  KEEP(*("._k_queue.static.*"))
  KEEP(*("._k_fifo.static.*"))
  KEEP(*("._k_lifo.static.*"))
  _k_queue_list_end = .;
 } > SRAM AT> FLASH
 _k_stack_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_stack_list_start = .;
  KEEP(*("._k_stack.static.*"))
  _k_stack_list_end = .;
 } > SRAM AT> FLASH
 _k_msgq_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_msgq_list_start = .;
  KEEP(*("._k_msgq.static.*"))
  _k_msgq_list_end = .;
 } > SRAM AT> FLASH
 _k_mbox_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_mbox_list_start = .;
  KEEP(*("._k_mbox.static.*"))
  _k_mbox_list_end = .;
 } > SRAM AT> FLASH
 _k_pipe_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _k_pipe_list_start = .;
  KEEP(*("._k_pipe.static.*"))
  _k_pipe_list_end = .;
 } > SRAM AT> FLASH
 _net_buf_pool_area : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  _net_buf_pool_list = .;
  KEEP(*(SORT("._net_buf_pool.static.*")))
 } > SRAM AT> FLASH
 net_if : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  __net_if_start = .;
  *(".net_if.*")
  KEEP(*(SORT(".net_if.*")))
  __net_if_end = .;
 } > SRAM AT> FLASH
 net_if_dev : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  __net_if_dev_start = .;
  *(".net_if_dev.*")
  KEEP(*(SORT(".net_if_dev.*")))
  __net_if_dev_end = .;
 } > SRAM AT> FLASH
 net_l2_data : ALIGN_WITH_INPUT SUBALIGN(4)
 {
  __net_l2_data_start = .;
  *(".net_l2.data")
  KEEP(*(SORT(".net_l2.data*")))
  __net_l2_data_end = .;
 } > SRAM AT> FLASH
 _static_kernel_objects_end = .;
 kobject_data : ALIGN_WITH_INPUT
 {
 *(".kobject_data.data*")
        } > SRAM AT> FLASH
 priv_stacks_noinit : ALIGN_WITH_INPUT
 {
 z_priv_stacks_ram_start = .;
 *(".priv_stacks.noinit")
 z_priv_stacks_ram_end = .;
 } > SRAM AT> FLASH
    __data_ram_end = .;
    _image_ram_end = .;
    _end = .;
    __kernel_ram_end = 0x20000000 + (256 * 1K);
    __kernel_ram_size = __kernel_ram_end - __kernel_ram_start;
   
.intList :
{
 KEEP(*(.irq_info))
 KEEP(*(.intList))
} > IDT_LIST
 .stab 0 : { *(.stab) }
 .stabstr 0 : { *(.stabstr) }
 .stab.excl 0 : { *(.stab.excl) }
 .stab.exclstr 0 : { *(.stab.exclstr) }
 .stab.index 0 : { *(.stab.index) }
 .stab.indexstr 0 : { *(.stab.indexstr) }
 .gnu.build.attributes 0 : { *(.gnu.build.attributes .gnu.build.attributes.*) }
 .comment 0 : { *(.comment) }
 .debug 0 : { *(.debug) }
 .line 0 : { *(.line) }
 .debug_srcinfo 0 : { *(.debug_srcinfo) }
 .debug_sfnames 0 : { *(.debug_sfnames) }
 .debug_aranges 0 : { *(.debug_aranges) }
 .debug_pubnames 0 : { *(.debug_pubnames) }
 .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) }
 .debug_abbrev 0 : { *(.debug_abbrev) }
 .debug_line 0 : { *(.debug_line .debug_line.* .debug_line_end ) }
 .debug_frame 0 : { *(.debug_frame) }
 .debug_str 0 : { *(.debug_str) }
 .debug_loc 0 : { *(.debug_loc) }
 .debug_macinfo 0 : { *(.debug_macinfo) }
 .debug_weaknames 0 : { *(.debug_weaknames) }
 .debug_funcnames 0 : { *(.debug_funcnames) }
 .debug_typenames 0 : { *(.debug_typenames) }
 .debug_varnames 0 : { *(.debug_varnames) }
 .debug_pubtypes 0 : { *(.debug_pubtypes) }
 .debug_ranges 0 : { *(.debug_ranges) }
 .debug_macro 0 : { *(.debug_macro) }
    /DISCARD/ : { *(.note.GNU-stack) }
    .ARM.attributes 0 :
 {
 KEEP(*(.ARM.attributes))
 KEEP(*(.gnu.attributes))
 }
.last_section (NOLOAD) :
{
} > FLASH
_flash_used = LOADADDR(.last_section) - _image_rom_start;
    }
