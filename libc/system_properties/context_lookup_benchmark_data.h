/*
 * Copyright (C) 2020 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

static char oreo_property_contexts[] = R"(
net.rmnet               u:object_r:net_radio_prop:s0
net.gprs                u:object_r:net_radio_prop:s0
net.ppp                 u:object_r:net_radio_prop:s0
net.qmi                 u:object_r:net_radio_prop:s0
net.lte                 u:object_r:net_radio_prop:s0
net.cdma                u:object_r:net_radio_prop:s0
net.dns                 u:object_r:net_dns_prop:s0
sys.usb.config          u:object_r:system_radio_prop:s0
ril.                    u:object_r:radio_prop:s0
ro.ril.                 u:object_r:radio_prop:s0
gsm.                    u:object_r:radio_prop:s0
persist.radio           u:object_r:radio_prop:s0
net.                    u:object_r:system_prop:s0
dev.                    u:object_r:system_prop:s0
ro.runtime.             u:object_r:system_prop:s0
ro.runtime.firstboot    u:object_r:firstboot_prop:s0
hw.                     u:object_r:system_prop:s0
ro.hw.                  u:object_r:system_prop:s0
sys.                    u:object_r:system_prop:s0
sys.cppreopt            u:object_r:cppreopt_prop:s0
sys.powerctl            u:object_r:powerctl_prop:s0
sys.usb.ffs.            u:object_r:ffs_prop:s0
service.                u:object_r:system_prop:s0
dhcp.                   u:object_r:dhcp_prop:s0
dhcp.bt-pan.result      u:object_r:pan_result_prop:s0
bluetooth.              u:object_r:bluetooth_prop:s0
debug.                  u:object_r:debug_prop:s0
debug.db.               u:object_r:debuggerd_prop:s0
dumpstate.              u:object_r:dumpstate_prop:s0
dumpstate.options       u:object_r:dumpstate_options_prop:s0
log.                    u:object_r:log_prop:s0
log.tag                 u:object_r:log_tag_prop:s0
log.tag.WifiHAL         u:object_r:wifi_log_prop:s0
security.perf_harden    u:object_r:shell_prop:s0
service.adb.root        u:object_r:shell_prop:s0
service.adb.tcp.port    u:object_r:shell_prop:s0
persist.audio.          u:object_r:audio_prop:s0
persist.bluetooth.      u:object_r:bluetooth_prop:s0
persist.debug.          u:object_r:persist_debug_prop:s0
persist.logd.           u:object_r:logd_prop:s0
persist.logd.security   u:object_r:device_logging_prop:s0
persist.logd.logpersistd        u:object_r:logpersistd_logging_prop:s0
logd.logpersistd        u:object_r:logpersistd_logging_prop:s0
persist.log.tag         u:object_r:log_tag_prop:s0
persist.mmc.            u:object_r:mmc_prop:s0
persist.sys.            u:object_r:system_prop:s0
persist.sys.safemode    u:object_r:safemode_prop:s0
ro.sys.safemode         u:object_r:safemode_prop:s0
persist.sys.audit_safemode      u:object_r:safemode_prop:s0
persist.service.        u:object_r:system_prop:s0
persist.service.bdroid. u:object_r:bluetooth_prop:s0
persist.security.       u:object_r:system_prop:s0
persist.vendor.overlay.  u:object_r:overlay_prop:s0
ro.boot.vendor.overlay.  u:object_r:overlay_prop:s0
ro.boottime.             u:object_r:boottime_prop:s0
ro.serialno             u:object_r:serialno_prop:s0
ro.boot.btmacaddr       u:object_r:bluetooth_prop:s0
ro.boot.serialno        u:object_r:serialno_prop:s0
ro.bt.                  u:object_r:bluetooth_prop:s0
# Boolean property set by system server upon boot indicating
# if device owner is provisioned.
ro.device_owner         u:object_r:device_logging_prop:s0
# selinux non-persistent properties
selinux.restorecon_recursive   u:object_r:restorecon_prop:s0
# default property context
*                       u:object_r:default_prop:s0
# data partition encryption properties
vold.                   u:object_r:vold_prop:s0
ro.crypto.              u:object_r:vold_prop:s0
# ro.build.fingerprint is either set in /system/build.prop, or is
# set at runtime by system_server.
ro.build.fingerprint    u:object_r:fingerprint_prop:s0
ro.persistent_properties.ready  u:object_r:persistent_properties_ready_prop:s0
# ctl properties
ctl.bootanim            u:object_r:ctl_bootanim_prop:s0
ctl.dumpstate           u:object_r:ctl_dumpstate_prop:s0
ctl.fuse_               u:object_r:ctl_fuse_prop:s0
ctl.mdnsd               u:object_r:ctl_mdnsd_prop:s0
ctl.ril-daemon          u:object_r:ctl_rildaemon_prop:s0
ctl.bugreport           u:object_r:ctl_bugreport_prop:s0
ctl.console             u:object_r:ctl_console_prop:s0
ctl.                    u:object_r:ctl_default_prop:s0
# NFC properties
nfc.                    u:object_r:nfc_prop:s0
# These properties are not normally set by processes other than init.
# They are only distinguished here for setting by qemu-props on the
# emulator/goldfish.
config.                 u:object_r:config_prop:s0
ro.config.              u:object_r:config_prop:s0
dalvik.                 u:object_r:dalvik_prop:s0
ro.dalvik.              u:object_r:dalvik_prop:s0
# Shared between system server and wificond
wlan.                   u:object_r:wifi_prop:s0
# hwservicemanager properties
hwservicemanager.       u:object_r:hwservicemanager_prop:s0
# ASAN install trigger
asan.restore_reboot  u:object_r:asan_reboot_prop:s0)";

static char aosp_s_property_contexts[] = R"(
net.rmnet               u:object_r:net_radio_prop:s0
net.gprs                u:object_r:net_radio_prop:s0
net.ppp                 u:object_r:net_radio_prop:s0
net.qmi                 u:object_r:net_radio_prop:s0
net.lte                 u:object_r:net_radio_prop:s0
net.cdma                u:object_r:net_radio_prop:s0
net.dns                 u:object_r:net_dns_prop:s0
ril.                    u:object_r:radio_prop:s0
ro.ril.                 u:object_r:radio_prop:s0
gsm.                    u:object_r:radio_prop:s0
persist.radio           u:object_r:radio_prop:s0

net.                    u:object_r:system_prop:s0
dev.                    u:object_r:system_prop:s0
ro.runtime.             u:object_r:system_prop:s0
ro.runtime.firstboot    u:object_r:firstboot_prop:s0
hw.                     u:object_r:system_prop:s0
ro.hw.                  u:object_r:system_prop:s0
sys.                    u:object_r:system_prop:s0
sys.audio.              u:object_r:audio_prop:s0
sys.init.perf_lsm_hooks u:object_r:init_perf_lsm_hooks_prop:s0
sys.cppreopt            u:object_r:cppreopt_prop:s0
sys.lpdumpd             u:object_r:lpdumpd_prop:s0
sys.powerctl            u:object_r:powerctl_prop:s0
service.                u:object_r:system_prop:s0
dhcp.                   u:object_r:dhcp_prop:s0
dhcp.bt-pan.result      u:object_r:pan_result_prop:s0
bluetooth.              u:object_r:bluetooth_prop:s0

debug.                  u:object_r:debug_prop:s0
debug.db.               u:object_r:debuggerd_prop:s0
dumpstate.              u:object_r:dumpstate_prop:s0
dumpstate.options       u:object_r:dumpstate_options_prop:s0
init.svc_debug_pid.     u:object_r:init_svc_debug_prop:s0
llk.                    u:object_r:llkd_prop:s0
khungtask.              u:object_r:llkd_prop:s0
ro.llk.                 u:object_r:llkd_prop:s0
ro.khungtask.           u:object_r:llkd_prop:s0
log.                    u:object_r:log_prop:s0
log.tag                 u:object_r:log_tag_prop:s0
log.tag.WifiHAL         u:object_r:wifi_log_prop:s0
security.perf_harden    u:object_r:shell_prop:s0
service.adb.root        u:object_r:shell_prop:s0
service.adb.tls.port    u:object_r:adbd_prop:s0
persist.adb.wifi.       u:object_r:adbd_prop:s0
persist.adb.tls_server.enable  u:object_r:system_adbd_prop:s0

persist.audio.          u:object_r:audio_prop:s0
persist.bluetooth.      u:object_r:bluetooth_prop:s0
persist.nfc_cfg.        u:object_r:nfc_prop:s0
persist.debug.          u:object_r:persist_debug_prop:s0
logd.                   u:object_r:logd_prop:s0
persist.logd.           u:object_r:logd_prop:s0
ro.logd.                u:object_r:logd_prop:s0
persist.logd.security   u:object_r:device_logging_prop:s0
persist.logd.logpersistd        u:object_r:logpersistd_logging_prop:s0
logd.logpersistd        u:object_r:logpersistd_logging_prop:s0
persist.log.tag         u:object_r:log_tag_prop:s0
persist.mmc.            u:object_r:mmc_prop:s0
persist.netd.stable_secret      u:object_r:netd_stable_secret_prop:s0
persist.pm.mock-upgrade u:object_r:mock_ota_prop:s0
persist.sys.            u:object_r:system_prop:s0
persist.sys.safemode    u:object_r:safemode_prop:s0
persist.sys.theme       u:object_r:theme_prop:s0
persist.sys.fflag.override.settings_dynamic_system    u:object_r:dynamic_system_prop:s0
ro.sys.safemode         u:object_r:safemode_prop:s0
persist.sys.audit_safemode      u:object_r:safemode_prop:s0
persist.sys.dalvik.jvmtiagent   u:object_r:system_jvmti_agent_prop:s0
persist.service.        u:object_r:system_prop:s0
persist.service.bdroid. u:object_r:bluetooth_prop:s0
persist.security.       u:object_r:system_prop:s0
persist.traced.enable   u:object_r:traced_enabled_prop:s0
traced.lazy.            u:object_r:traced_lazy_prop:s0
persist.heapprofd.enable u:object_r:heapprofd_enabled_prop:s0
persist.traced_perf.enable u:object_r:traced_perf_enabled_prop:s0
persist.vendor.overlay.  u:object_r:overlay_prop:s0
ro.boot.vendor.overlay.  u:object_r:overlay_prop:s0
ro.boottime.             u:object_r:boottime_prop:s0
ro.serialno             u:object_r:serialno_prop:s0
ro.boot.btmacaddr       u:object_r:bluetooth_prop:s0
ro.boot.serialno        u:object_r:serialno_prop:s0
ro.bt.                  u:object_r:bluetooth_prop:s0
ro.boot.bootreason      u:object_r:bootloader_boot_reason_prop:s0
persist.sys.boot.reason u:object_r:last_boot_reason_prop:s0
sys.boot.reason         u:object_r:system_boot_reason_prop:s0
sys.boot.reason.last    u:object_r:last_boot_reason_prop:s0
pm.                     u:object_r:pm_prop:s0
test.sys.boot.reason    u:object_r:test_boot_reason_prop:s0
test.userspace_reboot.requested u:object_r:userspace_reboot_test_prop:s0
sys.lmk.                u:object_r:system_lmk_prop:s0
sys.trace.              u:object_r:system_trace_prop:s0

# Fastbootd protocol control property
fastbootd.protocol    u:object_r:fastbootd_protocol_prop:s0 exact enum usb tcp

# adbd protoctl configuration property
service.adb.tcp.port    u:object_r:adbd_config_prop:s0 exact int

# Boolean property set by system server upon boot indicating
# if device is fully owned by organization instead of being
# a personal device.
ro.organization_owned   u:object_r:device_logging_prop:s0

# selinux non-persistent properties
selinux.restorecon_recursive   u:object_r:restorecon_prop:s0

# default property context
*                       u:object_r:default_prop:s0

# data partition encryption properties
vold.                   u:object_r:vold_prop:s0
ro.crypto.              u:object_r:vold_prop:s0

# ro.build.fingerprint is either set in /system/build.prop, or is
# set at runtime by system_server.
ro.build.fingerprint    u:object_r:fingerprint_prop:s0 exact string

ro.persistent_properties.ready  u:object_r:persistent_properties_ready_prop:s0

# ctl properties
ctl.bootanim            u:object_r:ctl_bootanim_prop:s0
ctl.dumpstate           u:object_r:ctl_dumpstate_prop:s0
ctl.fuse_               u:object_r:ctl_fuse_prop:s0
ctl.mdnsd               u:object_r:ctl_mdnsd_prop:s0
ctl.ril-daemon          u:object_r:ctl_rildaemon_prop:s0
ctl.bugreport           u:object_r:ctl_bugreport_prop:s0
ctl.console             u:object_r:ctl_console_prop:s0
ctl.                    u:object_r:ctl_default_prop:s0

# Don't allow uncontrolled access to all services
ctl.sigstop_on$         u:object_r:ctl_sigstop_prop:s0
ctl.sigstop_off$        u:object_r:ctl_sigstop_prop:s0
ctl.start$              u:object_r:ctl_start_prop:s0
ctl.stop$               u:object_r:ctl_stop_prop:s0
ctl.restart$            u:object_r:ctl_restart_prop:s0
ctl.interface_start$    u:object_r:ctl_interface_start_prop:s0
ctl.interface_stop$     u:object_r:ctl_interface_stop_prop:s0
ctl.interface_restart$  u:object_r:ctl_interface_restart_prop:s0

 # Restrict access to starting/stopping adbd
ctl.start$adbd             u:object_r:ctl_adbd_prop:s0
ctl.stop$adbd              u:object_r:ctl_adbd_prop:s0
ctl.restart$adbd           u:object_r:ctl_adbd_prop:s0

# Restrict access to starting/stopping gsid.
ctl.start$gsid          u:object_r:ctl_gsid_prop:s0
ctl.stop$gsid           u:object_r:ctl_gsid_prop:s0
ctl.restart$gsid        u:object_r:ctl_gsid_prop:s0

# Restrict access to stopping apexd.
ctl.stop$apexd          u:object_r:ctl_apexd_prop:s0

# Restrict access to restart dumpstate
ctl.interface_restart$android.hardware.dumpstate u:object_r:ctl_dumpstate_prop:s0

# NFC properties
nfc.                    u:object_r:nfc_prop:s0

# These properties are not normally set by processes other than init.
# They are only distinguished here for setting by qemu-props on the
# emulator/goldfish.
config.                 u:object_r:config_prop:s0
ro.config.              u:object_r:config_prop:s0
dalvik.                 u:object_r:dalvik_prop:s0
ro.dalvik.              u:object_r:dalvik_prop:s0

# Shared between system server and wificond
wifi.                   u:object_r:wifi_prop:s0
wlan.                   u:object_r:wifi_prop:s0

# Lowpan properties
lowpan.                 u:object_r:lowpan_prop:s0
ro.lowpan.              u:object_r:lowpan_prop:s0

# heapprofd properties
heapprofd.              u:object_r:heapprofd_prop:s0

# hwservicemanager properties
hwservicemanager.       u:object_r:hwservicemanager_prop:s0

# Common default properties for vendor, odm, vendor_dlkm, and odm_dlkm.
init.svc.odm.           u:object_r:vendor_default_prop:s0
init.svc.vendor.        u:object_r:vendor_default_prop:s0
ro.hardware.            u:object_r:vendor_default_prop:s0
ro.odm.                 u:object_r:vendor_default_prop:s0
ro.vendor.              u:object_r:vendor_default_prop:s0
ro.vendor_dlkm.         u:object_r:vendor_default_prop:s0
ro.odm_dlkm.            u:object_r:vendor_default_prop:s0
odm.                    u:object_r:vendor_default_prop:s0
persist.odm.            u:object_r:vendor_default_prop:s0
persist.vendor.         u:object_r:vendor_default_prop:s0
vendor.                 u:object_r:vendor_default_prop:s0

# Properties that relate to time / timezone detection behavior.
persist.time.           u:object_r:time_prop:s0

# Properties that relate to server configurable flags
device_config.reset_performed           u:object_r:device_config_reset_performed_prop:s0
persist.device_config.activity_manager_native_boot. u:object_r:device_config_activity_manager_native_boot_prop:s0
persist.device_config.attempted_boot_count        u:object_r:device_config_boot_count_prop:s0
persist.device_config.input_native_boot. u:object_r:device_config_input_native_boot_prop:s0
persist.device_config.netd_native.           u:object_r:device_config_netd_native_prop:s0
persist.device_config.runtime_native.        u:object_r:device_config_runtime_native_prop:s0
persist.device_config.runtime_native_boot.   u:object_r:device_config_runtime_native_boot_prop:s0
persist.device_config.media_native.          u:object_r:device_config_media_native_prop:s0
persist.device_config.storage_native_boot.   u:object_r:device_config_storage_native_boot_prop:s0
persist.device_config.window_manager_native_boot. u:object_r:device_config_window_manager_native_boot_prop:s0
persist.device_config.configuration. u:object_r:device_config_configuration_prop:s0

# Properties that relate to legacy server configurable flags
persist.device_config.global_settings.sys_traced u:object_r:device_config_sys_traced_prop:s0

apexd.                  u:object_r:apexd_prop:s0
persist.apexd.          u:object_r:apexd_prop:s0

bpf.progs_loaded        u:object_r:bpf_progs_loaded_prop:s0

gsid.                   u:object_r:gsid_prop:s0
ro.gsid.                u:object_r:gsid_prop:s0

# Property for disabling NNAPI vendor extensions on product image (used on GSI /product image,
# which can't use NNAPI vendor extensions).
ro.nnapi.extensions.deny_on_product                u:object_r:nnapi_ext_deny_product_prop:s0

# Property that is set once ueventd finishes cold boot.
ro.cold_boot_done       u:object_r:cold_boot_done_prop:s0

# Charger properties
ro.charger.                 u:object_r:charger_prop:s0
sys.boot_from_charger_mode  u:object_r:charger_status_prop:s0 exact int
ro.enable_boot_charger_mode u:object_r:charger_config_prop:s0 exact bool

# Virtual A/B properties
ro.virtual_ab.enabled   u:object_r:virtual_ab_prop:s0
ro.virtual_ab.retrofit  u:object_r:virtual_ab_prop:s0

ro.product.ab_ota_partitions u:object_r:ota_prop:s0 exact string
# Property to set/clear the warm reset flag after an OTA update.
ota.warm_reset  u:object_r:ota_prop:s0

# Module properties
com.android.sdkext.                  u:object_r:module_sdkextensions_prop:s0
persist.com.android.sdkext.          u:object_r:module_sdkextensions_prop:s0

# Userspace reboot properties
sys.userspace_reboot.log.         u:object_r:userspace_reboot_log_prop:s0
persist.sys.userspace_reboot.log. u:object_r:userspace_reboot_log_prop:s0

# Integer property which is used in libgui to configure the number of frames
# tracked by buffer queue's frame event timing history. The property is set
# by devices with video decoding pipelines long enough to overflow the default
# history size.
ro.lib_gui.frame_event_history_size u:object_r:bq_config_prop:s0

af.fast_track_multiplier     u:object_r:audio_config_prop:s0 exact int
ro.af.client_heap_size_kbyte u:object_r:audio_config_prop:s0 exact int

audio.camerasound.force         u:object_r:audio_config_prop:s0 exact bool
audio.deep_buffer.media         u:object_r:audio_config_prop:s0 exact bool
audio.offload.video             u:object_r:audio_config_prop:s0 exact bool
audio.offload.min.duration.secs u:object_r:audio_config_prop:s0 exact int

ro.audio.ignore_effects  u:object_r:audio_config_prop:s0 exact bool
ro.audio.monitorRotation u:object_r:audio_config_prop:s0 exact bool

persist.config.calibration_fac u:object_r:camera_calibration_prop:s0 exact string

config.disable_cameraservice u:object_r:camera_config_prop:s0 exact bool

camera.disable_zsl_mode u:object_r:camera_config_prop:s0 exact bool
camera.fifo.disable     u:object_r:camera_config_prop:s0 exact bool
ro.camera.notify_nfc    u:object_r:camera_config_prop:s0 exact bool
ro.camera.enableLazyHal u:object_r:camera_config_prop:s0 exact bool

# Should always_debuggable be bool? It's checked against the string "1".
dalvik.vm.always_debuggable                   u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.appimageformat                      u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.backgroundgctype                    u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.boot-dex2oat-cpu-set                u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.boot-dex2oat-threads                u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.boot-image                          u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.checkjni                            u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.dex2oat-Xms                         u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.dex2oat-Xmx                         u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.dex2oat-cpu-set                     u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.dex2oat-filter                      u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.dex2oat-flags                       u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.dex2oat-max-image-block-size        u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.dex2oat-minidebuginfo               u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.dex2oat-resolve-startup-strings     u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.dex2oat-threads                     u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.dex2oat-updatable-bcp-packages-file u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.dex2oat-very-large                  u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.dex2oat-swap                        u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.dex2oat64.enabled                   u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.dexopt.secondary                    u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.execution-mode                      u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.extra-opts                          u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.foreground-heap-growth-multiplier   u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.gctype                              u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.heapgrowthlimit                     u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.heapmaxfree                         u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.heapminfree                         u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.heapsize                            u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.heapstartsize                       u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.heaptargetutilization               u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.hot-startup-method-samples          u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.image-dex2oat-Xms                   u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.image-dex2oat-Xmx                   u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.image-dex2oat-cpu-set               u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.image-dex2oat-filter                u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.image-dex2oat-flags                 u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.image-dex2oat-threads               u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.isa.arm.features                    u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.arm.variant                     u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.arm64.features                  u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.arm64.variant                   u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.mips.features                   u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.mips.variant                    u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.mips64.features                 u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.mips64.variant                  u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.unknown.features                u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.unknown.variant                 u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.x86.features                    u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.x86.variant                     u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.x86_64.features                 u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.isa.x86_64.variant                  u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.jitinitialsize                      u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.jitmaxsize                          u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.jitprithreadweight                  u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.jitthreshold                        u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.jittransitionweight                 u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.jniopts                             u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.lockprof.threshold                  u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.method-trace                        u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.method-trace-file                   u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.method-trace-file-siz               u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.method-trace-stream                 u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.profilesystemserver                 u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.profilebootclasspath                u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.restore-dex2oat-cpu-set             u:object_r:dalvik_config_prop:s0 exact string
dalvik.vm.restore-dex2oat-threads             u:object_r:dalvik_config_prop:s0 exact int
dalvik.vm.usejit                              u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.usejitprofiles                      u:object_r:dalvik_config_prop:s0 exact bool
dalvik.vm.zygote.max-boot-retry               u:object_r:dalvik_config_prop:s0 exact int
ro.zygote                                     u:object_r:dalvik_config_prop:s0 exact string

persist.sys.dalvik.vm.lib.2 u:object_r:dalvik_runtime_prop:s0 exact string

keyguard.no_require_sim u:object_r:keyguard_config_prop:s0 exact bool

media.recorder.show_manufacturer_and_model   u:object_r:media_config_prop:s0 exact bool
media.stagefright.cache-params               u:object_r:media_config_prop:s0 exact string
media.stagefright.thumbnail.prefer_hw_codecs u:object_r:media_config_prop:s0 exact bool
persist.sys.media.avsync                     u:object_r:media_config_prop:s0 exact bool

persist.bluetooth.a2dp_offload.cap             u:object_r:bluetooth_a2dp_offload_prop:s0 exact string
persist.bluetooth.a2dp_offload.disabled        u:object_r:bluetooth_a2dp_offload_prop:s0 exact bool
persist.bluetooth.bluetooth_audio_hal.disabled u:object_r:bluetooth_audio_hal_prop:s0 exact bool
persist.bluetooth.btsnoopenable                u:object_r:exported_bluetooth_prop:s0 exact bool

persist.radio.multisim.config u:object_r:radio_control_prop:s0 exact string

persist.sys.hdmi.keep_awake              u:object_r:hdmi_config_prop:s0 exact bool
ro.hdmi.cec_device_types                 u:object_r:hdmi_config_prop:s0 exact string
ro.hdmi.device_type                      u:object_r:hdmi_config_prop:s0 exact string
ro.hdmi.wake_on_hotplug                  u:object_r:hdmi_config_prop:s0 exact bool
ro.hdmi.cec.source.send_standby_on_sleep u:object_r:hdmi_config_prop:s0 exact enum to_tv broadcast none

pm.dexopt.ab-ota                        u:object_r:exported_pm_prop:s0 exact string
pm.dexopt.bg-dexopt                     u:object_r:exported_pm_prop:s0 exact string
pm.dexopt.boot                          u:object_r:exported_pm_prop:s0 exact string
pm.dexopt.disable_bg_dexopt             u:object_r:exported_pm_prop:s0 exact bool
pm.dexopt.downgrade_after_inactive_days u:object_r:exported_pm_prop:s0 exact int
pm.dexopt.first-boot                    u:object_r:exported_pm_prop:s0 exact string
pm.dexopt.inactive                      u:object_r:exported_pm_prop:s0 exact string
pm.dexopt.install                       u:object_r:exported_pm_prop:s0 exact string
pm.dexopt.shared                        u:object_r:exported_pm_prop:s0 exact string

ro.apk_verity.mode u:object_r:apk_verity_prop:s0 exact int

ro.bluetooth.a2dp_offload.supported u:object_r:bluetooth_a2dp_offload_prop:s0 exact bool

ro.boot.vendor.overlay.theme u:object_r:exported_overlay_prop:s0 exact string

ro.bt.bdaddr_path u:object_r:exported_bluetooth_prop:s0 exact string

ro.config.alarm_alert         u:object_r:systemsound_config_prop:s0 exact string
ro.config.alarm_vol_default   u:object_r:systemsound_config_prop:s0 exact int
ro.config.alarm_vol_steps     u:object_r:systemsound_config_prop:s0 exact int
ro.config.media_vol_default   u:object_r:systemsound_config_prop:s0 exact int
ro.config.media_vol_steps     u:object_r:systemsound_config_prop:s0 exact int
ro.config.notification_sound  u:object_r:systemsound_config_prop:s0 exact string
ro.config.ringtone            u:object_r:systemsound_config_prop:s0 exact string
ro.config.system_vol_default  u:object_r:systemsound_config_prop:s0 exact int
ro.config.system_vol_steps    u:object_r:systemsound_config_prop:s0 exact int
ro.config.vc_call_vol_default u:object_r:systemsound_config_prop:s0 exact int

ro.control_privapp_permissions u:object_r:packagemanager_config_prop:s0 exact enum disable enforce log
ro.cp_system_other_odex        u:object_r:packagemanager_config_prop:s0 exact bool

ro.crypto.allow_encrypt_override                u:object_r:vold_config_prop:s0 exact bool
ro.crypto.dm_default_key.options_format.version u:object_r:vold_config_prop:s0 exact int
ro.crypto.fde_algorithm                         u:object_r:vold_config_prop:s0 exact string
ro.crypto.fde_sector_size                       u:object_r:vold_config_prop:s0 exact int
ro.crypto.scrypt_params                         u:object_r:vold_config_prop:s0 exact string
ro.crypto.set_dun                               u:object_r:vold_config_prop:s0 exact bool
ro.crypto.volume.contents_mode                  u:object_r:vold_config_prop:s0 exact string
ro.crypto.volume.filenames_mode                 u:object_r:vold_config_prop:s0 exact string
ro.crypto.volume.metadata.encryption            u:object_r:vold_config_prop:s0 exact string
ro.crypto.volume.metadata.method                u:object_r:vold_config_prop:s0 exact string
ro.crypto.volume.options                        u:object_r:vold_config_prop:s0 exact string

ro.dalvik.vm.native.bridge u:object_r:dalvik_config_prop:s0 exact string

external_storage.projid.enabled   u:object_r:storage_config_prop:s0 exact bool
external_storage.casefold.enabled u:object_r:storage_config_prop:s0 exact bool
external_storage.sdcardfs.enabled u:object_r:storage_config_prop:s0 exact bool

ro.config.per_app_memcg         u:object_r:lmkd_config_prop:s0 exact bool
ro.lmk.critical                 u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.critical_upgrade         u:object_r:lmkd_config_prop:s0 exact bool
ro.lmk.debug                    u:object_r:lmkd_config_prop:s0 exact bool
ro.lmk.downgrade_pressure       u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.kill_heaviest_task       u:object_r:lmkd_config_prop:s0 exact bool
ro.lmk.kill_timeout_ms          u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.low                      u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.medium                   u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.psi_partial_stall_ms     u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.psi_complete_stall_ms    u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.swap_free_low_percentage u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.swap_util_max            u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.thrashing_limit          u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.thrashing_limit_decay    u:object_r:lmkd_config_prop:s0 exact int
ro.lmk.use_minfree_levels       u:object_r:lmkd_config_prop:s0 exact bool
ro.lmk.upgrade_pressure         u:object_r:lmkd_config_prop:s0 exact int
lmkd.reinit                     u:object_r:lmkd_prop:s0 exact int

ro.media.xml_variant.codecs             u:object_r:media_variant_prop:s0 exact string
ro.media.xml_variant.codecs_performance u:object_r:media_variant_prop:s0 exact string
ro.media.xml_variant.profiles           u:object_r:media_variant_prop:s0 exact string

ro.minui.default_rotation u:object_r:recovery_config_prop:s0 exact string
ro.minui.overscan_percent u:object_r:recovery_config_prop:s0 exact int
ro.minui.pixel_format     u:object_r:recovery_config_prop:s0 exact string

ro.oem_unlock_supported u:object_r:oem_unlock_prop:s0 exact int

ro.rebootescrow.device u:object_r:rebootescrow_hal_prop:s0 exact string

ro.storage_manager.enabled u:object_r:storagemanager_config_prop:s0 exact bool

ro.vehicle.hal u:object_r:vehicle_hal_prop:s0 exact string

ro.vendor.build.security_patch u:object_r:vendor_security_patch_level_prop:s0 exact string

ro.zram.mark_idle_delay_mins    u:object_r:zram_config_prop:s0 exact int
ro.zram.first_wb_delay_mins     u:object_r:zram_config_prop:s0 exact int
ro.zram.periodic_wb_delay_hours u:object_r:zram_config_prop:s0 exact int
zram.force_writeback            u:object_r:zram_config_prop:s0 exact bool
persist.sys.zram_enabled        u:object_r:zram_control_prop:s0 exact bool

sendbug.preferred.domain u:object_r:sendbug_config_prop:s0 exact string

persist.sys.usb.usbradio.config u:object_r:usb_control_prop:s0 exact string

sys.usb.config     u:object_r:usb_control_prop:s0 exact string
sys.usb.configfs   u:object_r:usb_control_prop:s0 exact int
sys.usb.controller u:object_r:usb_control_prop:s0 exact string
sys.usb.state      u:object_r:usb_control_prop:s0 exact string

sys.usb.mtp.device_type u:object_r:usb_config_prop:s0 exact int

sys.usb.config. u:object_r:usb_prop:s0

sys.usb.ffs.aio_compat u:object_r:ffs_config_prop:s0 exact bool
sys.usb.ffs.max_read   u:object_r:ffs_config_prop:s0 exact int
sys.usb.ffs.max_write  u:object_r:ffs_config_prop:s0 exact int

sys.usb.ffs.ready     u:object_r:ffs_control_prop:s0 exact bool
sys.usb.ffs.mtp.ready u:object_r:ffs_control_prop:s0 exact bool

tombstoned.max_tombstone_count u:object_r:tombstone_config_prop:s0 exact int

vold.post_fs_data_done u:object_r:vold_config_prop:s0 exact int

apexd.status u:object_r:apexd_prop:s0 exact enum starting activated ready

dev.bootcomplete   u:object_r:boot_status_prop:s0 exact bool
sys.boot_completed u:object_r:boot_status_prop:s0 exact bool

persist.sys.device_provisioned u:object_r:provisioned_prop:s0 exact string

persist.sys.theme               u:object_r:theme_prop:s0 exact string

sys.retaildemo.enabled u:object_r:retaildemo_prop:s0 exact int

sys.user.0.ce_available u:object_r:exported3_system_prop:s0 exact bool

aac_drc_boost            u:object_r:aac_drc_prop:s0 exact int
aac_drc_cut              u:object_r:aac_drc_prop:s0 exact int
aac_drc_enc_target_level u:object_r:aac_drc_prop:s0 exact int
aac_drc_heavy            u:object_r:aac_drc_prop:s0 exact int
aac_drc_reference_level  u:object_r:aac_drc_prop:s0 exact int
ro.aac_drc_effect_type   u:object_r:aac_drc_prop:s0 exact int

build.version.extensions. u:object_r:module_sdkextensions_prop:s0 prefix int

drm.64bit.enabled            u:object_r:mediadrm_config_prop:s0 exact bool
media.mediadrmservice.enable u:object_r:mediadrm_config_prop:s0 exact bool

drm.service.enabled u:object_r:drm_service_config_prop:s0 exact bool

dumpstate.dry_run u:object_r:exported_dumpstate_prop:s0 exact bool
dumpstate.unroot  u:object_r:exported_dumpstate_prop:s0 exact bool
persist.dumpstate.verbose_logging.enabled u:object_r:hal_dumpstate_config_prop:s0 exact bool

hal.instrumentation.enable u:object_r:hal_instrumentation_prop:s0 exact bool

# default contexts only accessible by coredomain
init.svc. u:object_r:init_service_status_private_prop:s0 prefix string

# vendor-init-readable init service props
init.svc.bugreport      u:object_r:init_service_status_prop:s0 exact string
init.svc.bugreportd     u:object_r:init_service_status_prop:s0 exact string
init.svc.console        u:object_r:init_service_status_prop:s0 exact string
init.svc.dumpstatez     u:object_r:init_service_status_prop:s0 exact string
init.svc.mediadrm       u:object_r:init_service_status_prop:s0 exact string
init.svc.statsd         u:object_r:init_service_status_prop:s0 exact string
init.svc.surfaceflinger u:object_r:init_service_status_prop:s0 exact string
init.svc.tombstoned     u:object_r:init_service_status_prop:s0 exact string
init.svc.zygote         u:object_r:init_service_status_prop:s0 exact string

libc.debug.malloc.options u:object_r:libc_debug_prop:s0 exact string
libc.debug.malloc.program u:object_r:libc_debug_prop:s0 exact string
libc.debug.hooks.enable   u:object_r:libc_debug_prop:s0 exact string

net.redirect_socket_calls.hooked u:object_r:socket_hook_prop:s0 exact bool

persist.sys.locale       u:object_r:exported_system_prop:s0 exact string
persist.sys.timezone     u:object_r:exported_system_prop:s0 exact string
persist.sys.test_harness u:object_r:test_harness_prop:s0 exact bool

ro.arch u:object_r:build_prop:s0 exact string

# ro.boot. properties are set based on kernel commandline arguments, which are vendor owned.
ro.boot.                   u:object_r:bootloader_prop:s0
ro.boot.avb_version        u:object_r:bootloader_prop:s0 exact string
ro.boot.baseband           u:object_r:bootloader_prop:s0 exact string
ro.boot.bootdevice         u:object_r:bootloader_prop:s0 exact string
ro.boot.bootloader         u:object_r:bootloader_prop:s0 exact string
ro.boot.boottime           u:object_r:bootloader_prop:s0 exact string
ro.boot.console            u:object_r:bootloader_prop:s0 exact string
ro.boot.hardware           u:object_r:bootloader_prop:s0 exact string
ro.boot.hardware.color     u:object_r:bootloader_prop:s0 exact string
ro.boot.hardware.sku       u:object_r:bootloader_prop:s0 exact string
ro.boot.keymaster          u:object_r:bootloader_prop:s0 exact string
ro.boot.mode               u:object_r:bootloader_prop:s0 exact string
ro.boot.revision           u:object_r:bootloader_prop:s0 exact string
ro.boot.vbmeta.avb_version u:object_r:bootloader_prop:s0 exact string
ro.boot.verifiedbootstate  u:object_r:bootloader_prop:s0 exact string
ro.boot.veritymode         u:object_r:bootloader_prop:s0 exact string

# These ro.X properties are set to values of ro.boot.X by property_service.
ro.baseband   u:object_r:bootloader_prop:s0 exact string
ro.bootloader u:object_r:bootloader_prop:s0 exact string
ro.bootmode   u:object_r:bootloader_prop:s0 exact string
ro.hardware   u:object_r:bootloader_prop:s0 exact string
ro.revision   u:object_r:bootloader_prop:s0 exact string

ro.boot.dynamic_partitions          u:object_r:exported_default_prop:s0 exact string
ro.boot.dynamic_partitions_retrofit u:object_r:exported_default_prop:s0 exact string

ro.build.date                        u:object_r:build_prop:s0 exact string
ro.build.date.utc                    u:object_r:build_prop:s0 exact int
ro.build.description                 u:object_r:build_prop:s0 exact string
ro.build.display.id                  u:object_r:build_prop:s0 exact string
ro.build.host                        u:object_r:build_prop:s0 exact string
ro.build.id                          u:object_r:build_prop:s0 exact string
ro.build.product                     u:object_r:build_prop:s0 exact string
ro.build.system_root_image           u:object_r:build_prop:s0 exact bool
ro.build.tags                        u:object_r:build_prop:s0 exact string
ro.build.type                        u:object_r:build_prop:s0 exact string
ro.build.user                        u:object_r:build_prop:s0 exact string
ro.build.version.base_os             u:object_r:build_prop:s0 exact string
ro.build.version.codename            u:object_r:build_prop:s0 exact string
ro.build.version.incremental         u:object_r:build_prop:s0 exact string
ro.build.version.preview_sdk         u:object_r:build_prop:s0 exact int
ro.build.version.release             u:object_r:build_prop:s0 exact string
ro.build.version.release_or_codename u:object_r:build_prop:s0 exact string
ro.build.version.sdk                 u:object_r:build_prop:s0 exact int
ro.build.version.security_patch      u:object_r:build_prop:s0 exact string

ro.debuggable u:object_r:build_prop:s0 exact bool

ro.product.cpu.abi       u:object_r:build_prop:s0 exact string
ro.product.cpu.abilist   u:object_r:build_prop:s0 exact string
ro.product.cpu.abilist32 u:object_r:build_prop:s0 exact string
ro.product.cpu.abilist64 u:object_r:build_prop:s0 exact string

ro.adb.secure u:object_r:build_prop:s0 exact bool
ro.secure     u:object_r:build_prop:s0 exact int

# These 5 properties are set by property_service
ro.product.brand         u:object_r:build_prop:s0 exact string
ro.product.device        u:object_r:build_prop:s0 exact string
ro.product.manufacturer  u:object_r:build_prop:s0 exact string
ro.product.model         u:object_r:build_prop:s0 exact string
ro.product.name          u:object_r:build_prop:s0 exact string

# Sanitizer properties
ro.sanitize.address          u:object_r:build_prop:s0 exact bool
ro.sanitize.cfi              u:object_r:build_prop:s0 exact bool
ro.sanitize.default-ub       u:object_r:build_prop:s0 exact bool
ro.sanitize.fuzzer           u:object_r:build_prop:s0 exact bool
ro.sanitize.hwaddress        u:object_r:build_prop:s0 exact bool
ro.sanitize.integer_overflow u:object_r:build_prop:s0 exact bool
ro.sanitize.safe-stack       u:object_r:build_prop:s0 exact bool
ro.sanitize.scudo            u:object_r:build_prop:s0 exact bool
ro.sanitize.thread           u:object_r:build_prop:s0 exact bool
ro.sanitize.undefined        u:object_r:build_prop:s0 exact bool

# All odm build props are set by /odm/build.prop
ro.odm.build.date                u:object_r:build_odm_prop:s0 exact string
ro.odm.build.date.utc            u:object_r:build_odm_prop:s0 exact int
ro.odm.build.fingerprint         u:object_r:build_odm_prop:s0 exact string
ro.odm.build.version.incremental u:object_r:build_odm_prop:s0 exact string

ro.product.odm.brand        u:object_r:build_odm_prop:s0 exact string
ro.product.odm.device       u:object_r:build_odm_prop:s0 exact string
ro.product.odm.manufacturer u:object_r:build_odm_prop:s0 exact string
ro.product.odm.model        u:object_r:build_odm_prop:s0 exact string
ro.product.odm.name         u:object_r:build_odm_prop:s0 exact string

# All vendor_dlkm build props are set by /vendor_dlkm/etc/build.prop
ro.vendor_dlkm.build.date                u:object_r:build_vendor_prop:s0 exact string
ro.vendor_dlkm.build.date.utc            u:object_r:build_vendor_prop:s0 exact int
ro.vendor_dlkm.build.fingerprint         u:object_r:build_vendor_prop:s0 exact string
ro.vendor_dlkm.build.version.incremental u:object_r:build_vendor_prop:s0 exact string

# All odm_dlkm build props are set by /odm_dlkm/etc/build.prop
ro.odm_dlkm.build.date                u:object_r:build_vendor_prop:s0 exact string
ro.odm_dlkm.build.date.utc            u:object_r:build_vendor_prop:s0 exact int
ro.odm_dlkm.build.fingerprint         u:object_r:build_vendor_prop:s0 exact string
ro.odm_dlkm.build.version.incremental u:object_r:build_vendor_prop:s0 exact string

# All vendor build props are set by /vendor/build.prop
ro.vendor.build.date                u:object_r:build_vendor_prop:s0 exact string
ro.vendor.build.date.utc            u:object_r:build_vendor_prop:s0 exact int
ro.vendor.build.fingerprint         u:object_r:build_vendor_prop:s0 exact string
ro.vendor.build.version.incremental u:object_r:build_vendor_prop:s0 exact string
ro.vendor.build.version.sdk         u:object_r:build_vendor_prop:s0 exact int

ro.product.board               u:object_r:build_vendor_prop:s0 exact string
ro.product.first_api_level     u:object_r:build_vendor_prop:s0 exact int
ro.product.vendor.brand        u:object_r:build_vendor_prop:s0 exact string
ro.product.vendor.device       u:object_r:build_vendor_prop:s0 exact string
ro.product.vendor.manufacturer u:object_r:build_vendor_prop:s0 exact string
ro.product.vendor.model        u:object_r:build_vendor_prop:s0 exact string
ro.product.vendor.name         u:object_r:build_vendor_prop:s0 exact string

ro.crypto.state u:object_r:vold_status_prop:s0 exact enum encrypted unencrypted unsupported
ro.crypto.type  u:object_r:vold_status_prop:s0 exact enum block file none

ro.property_service.version u:object_r:property_service_version_prop:s0 exact int

ro.vendor.redirect_socket_calls u:object_r:vendor_socket_hook_prop:s0 exact bool

service.bootanim.exit u:object_r:exported_system_prop:s0 exact int

sys.init.userspace_reboot.in_progress u:object_r:userspace_reboot_exported_prop:s0 exact bool
sys.use_memfd                         u:object_r:use_memfd_prop:s0 exact bool

vold.decrypt u:object_r:vold_status_prop:s0 exact string

aaudio.hw_burst_min_usec     u:object_r:aaudio_config_prop:s0 exact int
aaudio.minimum_sleep_usec    u:object_r:aaudio_config_prop:s0 exact int
aaudio.mixer_bursts          u:object_r:aaudio_config_prop:s0 exact int
aaudio.mmap_exclusive_policy u:object_r:aaudio_config_prop:s0 exact int
aaudio.mmap_policy           u:object_r:aaudio_config_prop:s0 exact int
aaudio.wakeup_delay_usec     u:object_r:aaudio_config_prop:s0 exact int

persist.rcs.supported u:object_r:exported_default_prop:s0 exact int

ro.bionic.2nd_arch        u:object_r:cpu_variant_prop:s0 exact string
ro.bionic.2nd_cpu_variant u:object_r:cpu_variant_prop:s0 exact string
ro.bionic.arch            u:object_r:cpu_variant_prop:s0 exact string
ro.bionic.cpu_variant     u:object_r:cpu_variant_prop:s0 exact string

ro.board.platform u:object_r:exported_default_prop:s0 exact string

ro.boot.fake_battery         u:object_r:exported_default_prop:s0 exact int
ro.boot.fstab_suffix         u:object_r:exported_default_prop:s0 exact string
ro.boot.hardware.revision    u:object_r:exported_default_prop:s0 exact string
ro.boot.product.hardware.sku u:object_r:exported_default_prop:s0 exact string
ro.boot.product.vendor.sku   u:object_r:exported_default_prop:s0 exact string
ro.boot.slot_suffix          u:object_r:exported_default_prop:s0 exact string

ro.boringcrypto.hwrand u:object_r:exported_default_prop:s0 exact bool

ro.build.ab_update         u:object_r:exported_default_prop:s0 exact string
ro.build.expect.baseband   u:object_r:exported_default_prop:s0 exact string
ro.build.expect.bootloader u:object_r:exported_default_prop:s0 exact string

ro.carrier u:object_r:exported_default_prop:s0 exact string

ro.config.low_ram           u:object_r:exported_config_prop:s0 exact bool
ro.config.vc_call_vol_steps u:object_r:exported_config_prop:s0 exact int

ro.frp.pst u:object_r:exported_default_prop:s0 exact string

ro.hardware.activity_recognition u:object_r:exported_default_prop:s0 exact string
ro.hardware.audio                u:object_r:exported_default_prop:s0 exact string
ro.hardware.audio.a2dp           u:object_r:exported_default_prop:s0 exact string
ro.hardware.audio.hearing_aid    u:object_r:exported_default_prop:s0 exact string
ro.hardware.audio.primary        u:object_r:exported_default_prop:s0 exact string
ro.hardware.audio.usb            u:object_r:exported_default_prop:s0 exact string
ro.hardware.audio_policy         u:object_r:exported_default_prop:s0 exact string
ro.hardware.bootctrl             u:object_r:exported_default_prop:s0 exact string
ro.hardware.camera               u:object_r:exported_default_prop:s0 exact string
ro.hardware.consumerir           u:object_r:exported_default_prop:s0 exact string
ro.hardware.context_hub          u:object_r:exported_default_prop:s0 exact string
ro.hardware.egl                  u:object_r:exported_default_prop:s0 exact string
ro.hardware.fingerprint          u:object_r:exported_default_prop:s0 exact string
ro.hardware.flp                  u:object_r:exported_default_prop:s0 exact string
ro.hardware.gatekeeper           u:object_r:exported_default_prop:s0 exact string
ro.hardware.gps                  u:object_r:exported_default_prop:s0 exact string
ro.hardware.gralloc              u:object_r:exported_default_prop:s0 exact string
ro.hardware.hdmi_cec             u:object_r:exported_default_prop:s0 exact string
ro.hardware.hwcomposer           u:object_r:exported_default_prop:s0 exact string
ro.hardware.input                u:object_r:exported_default_prop:s0 exact string
ro.hardware.keystore             u:object_r:exported_default_prop:s0 exact string
ro.hardware.keystore_desede      u:object_r:exported_default_prop:s0 exact string
ro.hardware.lights               u:object_r:exported_default_prop:s0 exact string
ro.hardware.local_time           u:object_r:exported_default_prop:s0 exact string
ro.hardware.memtrack             u:object_r:exported_default_prop:s0 exact string
ro.hardware.nfc                  u:object_r:exported_default_prop:s0 exact string
ro.hardware.nfc_nci              u:object_r:exported_default_prop:s0 exact string
ro.hardware.nfc_tag              u:object_r:exported_default_prop:s0 exact string
ro.hardware.nvram                u:object_r:exported_default_prop:s0 exact string
ro.hardware.power                u:object_r:exported_default_prop:s0 exact string
ro.hardware.radio                u:object_r:exported_default_prop:s0 exact string
ro.hardware.sensors              u:object_r:exported_default_prop:s0 exact string
ro.hardware.sound_trigger        u:object_r:exported_default_prop:s0 exact string
ro.hardware.thermal              u:object_r:exported_default_prop:s0 exact string
ro.hardware.tv_input             u:object_r:exported_default_prop:s0 exact string
ro.hardware.type                 u:object_r:exported_default_prop:s0 exact string
ro.hardware.vehicle              u:object_r:exported_default_prop:s0 exact string
ro.hardware.vibrator             u:object_r:exported_default_prop:s0 exact string
ro.hardware.virtual_device       u:object_r:exported_default_prop:s0 exact string
ro.hardware.vulkan               u:object_r:exported_default_prop:s0 exact string

ro.hwui.use_vulkan u:object_r:exported_default_prop:s0 exact bool

ro.kernel.qemu             u:object_r:exported_default_prop:s0 exact bool
ro.kernel.qemu.            u:object_r:exported_default_prop:s0
ro.kernel.android.bootanim u:object_r:exported_default_prop:s0 exact int

ro.oem.key1 u:object_r:exported_default_prop:s0 exact string

ro.product.vndk.version u:object_r:vndk_prop:s0 exact string

ro.vndk.lite    u:object_r:vndk_prop:s0 exact bool
ro.vndk.version u:object_r:vndk_prop:s0 exact string

ro.vts.coverage u:object_r:vts_config_prop:s0 exact int

vts.native_server.on u:object_r:vts_status_prop:s0 exact bool

wifi.active.interface     u:object_r:wifi_hal_prop:s0 exact string
wifi.aware.interface      u:object_r:wifi_hal_prop:s0 exact string
wifi.concurrent.interface u:object_r:wifi_hal_prop:s0 exact string
wifi.direct.interface     u:object_r:wifi_hal_prop:s0 exact string
wifi.interface            u:object_r:wifi_hal_prop:s0 exact string
wlan.driver.status        u:object_r:wifi_hal_prop:s0 exact enum ok unloaded

ro.boot.wificountrycode u:object_r:wifi_config_prop:s0 exact string

ro.apex.updatable u:object_r:exported_default_prop:s0 exact bool

# Property to enable incremental feature
ro.incremental.enable      u:object_r:incremental_prop:s0

# Properties to configure userspace reboot.
init.userspace_reboot.is_supported u:object_r:userspace_reboot_config_prop:s0 exact bool
init.userspace_reboot.sigkill.timeoutmillis u:object_r:userspace_reboot_config_prop:s0 exact int
init.userspace_reboot.sigterm.timeoutmillis u:object_r:userspace_reboot_config_prop:s0 exact int
init.userspace_reboot.started.timeoutmillis u:object_r:userspace_reboot_config_prop:s0 exact int
init.userspace_reboot.userdata_remount.timeoutmillis u:object_r:userspace_reboot_config_prop:s0 exact int
init.userspace_reboot.watchdog.timeoutmillis u:object_r:userspace_reboot_config_prop:s0 exact int

sys.shutdown.requested u:object_r:exported_system_prop:s0 exact string

# surfaceflinger properties
ro.surface_flinger.default_composition_dataspace          u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.default_composition_pixel_format       u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.force_hwc_copy_for_virtual_displays    u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.has_HDR_display                        u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.has_wide_color_display                 u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.max_frame_buffer_acquired_buffers      u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.max_graphics_height                    u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.max_graphics_width                     u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.max_virtual_display_dimension          u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.primary_display_orientation            u:object_r:surfaceflinger_prop:s0 exact enum ORIENTATION_0 ORIENTATION_180 ORIENTATION_270 ORIENTATION_90
ro.surface_flinger.present_time_offset_from_vsync_ns      u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.running_without_sync_framework         u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.start_graphics_allocator_service       u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.use_color_management                   u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.use_context_priority                   u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.use_vr_flinger                         u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.vsync_event_phase_offset_ns            u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.vsync_sf_event_phase_offset_ns         u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.wcg_composition_dataspace              u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.wcg_composition_pixel_format           u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.display_primary_red                    u:object_r:surfaceflinger_prop:s0 exact string
ro.surface_flinger.display_primary_green                  u:object_r:surfaceflinger_prop:s0 exact string
ro.surface_flinger.display_primary_blue                   u:object_r:surfaceflinger_prop:s0 exact string
ro.surface_flinger.display_primary_white                  u:object_r:surfaceflinger_prop:s0 exact string
ro.surface_flinger.protected_contents                     u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.set_idle_timer_ms                      u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.set_touch_timer_ms                     u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.set_display_power_timer_ms             u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.support_kernel_idle_timer              u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.use_smart_90_for_video                 u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.use_content_detection_for_refresh_rate u:object_r:surfaceflinger_prop:s0 exact bool
ro.surface_flinger.color_space_agnostic_dataspace         u:object_r:surfaceflinger_prop:s0 exact int
ro.surface_flinger.refresh_rate_switching                 u:object_r:surfaceflinger_prop:s0 exact bool

ro.sf.disable_triple_buffer u:object_r:surfaceflinger_prop:s0 exact bool
ro.sf.lcd_density           u:object_r:surfaceflinger_prop:s0 exact int

persist.sys.sf.color_mode       u:object_r:surfaceflinger_color_prop:s0 exact int
persist.sys.sf.color_saturation u:object_r:surfaceflinger_color_prop:s0 exact string
persist.sys.sf.native_mode      u:object_r:surfaceflinger_color_prop:s0 exact int

# Binder cache properties.  These are world-readable
cache_key.app_inactive                   u:object_r:binder_cache_system_server_prop:s0
cache_key.is_compat_change_enabled       u:object_r:binder_cache_system_server_prop:s0
cache_key.get_packages_for_uid           u:object_r:binder_cache_system_server_prop:s0
cache_key.has_system_feature             u:object_r:binder_cache_system_server_prop:s0
cache_key.is_interactive                 u:object_r:binder_cache_system_server_prop:s0
cache_key.is_power_save_mode             u:object_r:binder_cache_system_server_prop:s0
cache_key.is_user_unlocked               u:object_r:binder_cache_system_server_prop:s0
cache_key.volume_list                    u:object_r:binder_cache_system_server_prop:s0
cache_key.display_info                   u:object_r:binder_cache_system_server_prop:s0
cache_key.location_enabled               u:object_r:binder_cache_system_server_prop:s0
cache_key.package_info                   u:object_r:binder_cache_system_server_prop:s0

cache_key.bluetooth.                     u:object_r:binder_cache_bluetooth_server_prop:s0 prefix string
cache_key.system_server.                 u:object_r:binder_cache_system_server_prop:s0 prefix string
cache_key.telephony.                     u:object_r:binder_cache_telephony_server_prop:s0 prefix string

gsm.sim.operator.numeric       u:object_r:telephony_status_prop:s0 exact string
persist.radio.airplane_mode_on u:object_r:telephony_status_prop:s0 exact bool

ro.com.android.dataroaming        u:object_r:telephony_config_prop:s0 exact bool
ro.com.android.prov_mobiledata    u:object_r:telephony_config_prop:s0 exact bool
ro.radio.noril                    u:object_r:telephony_config_prop:s0 exact string
ro.telephony.call_ring.multiple   u:object_r:telephony_config_prop:s0 exact bool
ro.telephony.default_cdma_sub     u:object_r:telephony_config_prop:s0 exact int
ro.telephony.default_network      u:object_r:telephony_config_prop:s0 exact string
ro.telephony.iwlan_operation_mode u:object_r:telephony_config_prop:s0 exact enum default legacy AP-assisted
telephony.active_modems.max_count u:object_r:telephony_config_prop:s0 exact int
telephony.lteOnCdmaDevice         u:object_r:telephony_config_prop:s0 exact int
persist.dbg.volte_avail_ovr       u:object_r:telephony_config_prop:s0 exact int
persist.dbg.vt_avail_ovr          u:object_r:telephony_config_prop:s0 exact int
persist.dbg.wfc_avail_ovr         u:object_r:telephony_config_prop:s0 exact int

# System locale list filter configuration
ro.localization.locale_filter u:object_r:localization_prop:s0 exact string

# Graphics related properties
ro.opengles.version u:object_r:graphics_config_prop:s0 exact int

ro.gfx.driver.0        u:object_r:graphics_config_prop:s0 exact string
ro.gfx.driver.1        u:object_r:graphics_config_prop:s0 exact string
ro.gfx.angle.supported u:object_r:graphics_config_prop:s0 exact bool

graphics.gpu.profiler.support          u:object_r:graphics_config_prop:s0 exact bool
graphics.gpu.profiler.vulkan_layer_apk u:object_r:graphics_config_prop:s0 exact string
    )";
