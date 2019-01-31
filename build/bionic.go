// Copyright (C) 2019 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package bionic

import (
	"fmt"
	"io"
	"strings"

	"github.com/google/blueprint/proptools"

	"android/soong/android"
)

// bionic_mountpoint is a module type that is specialized to create
// mount points for Bionic files (libc, libdl, libm, and linker).
//
// With following description,
//
// bionic_mountpoint {
//    name: "libc.mountpoint",
//    stem: "libc.so",
//    src: "dummy_mountpoint",
//    library: true,
//    symlinks: ["libc.so"],
// }
//
// , the build system does following jobs:
//
// A mount point /bionic/lib[64]/libc.so is created. Its content
// is from the file 'dummy_mountpoint'.
//
// Then a symlink is created at /system/lib[64]/libc.so which points to
// the created mountpoint.
//
// At runtime, on the mount point, either bootstrap Bionic or default Bionic
// (which is from the runtime APEX) is mounted by the init process. The
// symlink exists to provide consistent legacy path for compatibility
// reason.
func init() {
	android.RegisterModuleType("bionic_mountpoint", bionicMountpointFactory)
}

type bionicMountpoint struct {
	android.ModuleBase
	properties bionicMountpointProperties

	outputFile android.Path
	pathInPartition string
	stem string
}

type bionicMountpointProperties struct {
	// The file that is installed as the mount point
	Src *string

	// True if the mount point is for a Bionic library such libc.so
	Library *bool
	// True if the mount point is for a Bionic binary such as linker
	Binary *bool

	// Base name of the mount point
	Stem *string `android:"arch_variant"`

	// Append to the name of the output
	Suffix *string `android:"arch_variant"`

	// Symlinks to the mountpoints from the system and recovery partitions
	// Symlinks names will have the same suffix as the mount point
	Symlinks []string
}

func (m *bionicMountpoint) DepsMutator(ctx android.BottomUpMutatorContext) {
	if Bool(m.properties.Library) == Bool(m.properties.Binary) {
		ctx.ModuleErrorf("either binary or library must be set to true")
		return
	}
	if m.properties.Stem == nil {
		ctx.PropertyErrorf("stem", "stem must be set")
		return
	}
	if m.properties.Src == nil {
		ctx.PropertyErrorf("src", "src must be set")
	}
	android.ExtractSourceDeps(ctx, m.properties.Src)
}

func (m *bionicMountpoint) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	if Bool(m.properties.Library) {
		m.pathInPartition = "lib"
		if m.Arch().ArchType.Multilib == "lib64" {
			m.pathInPartition = "lib64"
		}
	} else if Bool(m.properties.Binary) {
		m.pathInPartition = "bin"
	}

	m.stem = String(m.properties.Stem) + String(m.properties.Suffix)

	m.outputFile = ctx.ExpandSource(String(m.properties.Src), "src")
}

func (m *bionicMountpoint) AndroidMk() android.AndroidMkData {
	return android.AndroidMkData {
		Custom: func(w io.Writer, name, prefix, moduleDir string, data android.AndroidMkData) {
			if !m.Arch().Native {
				return
			}
			fmt.Fprintln(w, "\ninclude $(CLEAR_VARS)")
			fmt.Fprintln(w, "LOCAL_PATH :=", moduleDir)
			fmt.Fprintln(w, "LOCAL_MODULE :=", name)
			fmt.Fprintln(w, "LOCAL_USE_CLANG_LLD := false")
			fmt.Fprintln(w, "LOCAL_STRIP_MODULE := false")
			if Bool(m.properties.Library) {
				fmt.Fprintln(w, "LOCAL_MODULE_CLASS := SHARED_LIBRARIES")
			} else if Bool(m.properties.Binary) {
				fmt.Fprintln(w, "LOCAL_MODULE_CLASS := EXECUTABLES")
			}
			fmt.Fprintln(w, "LOCAL_MODULE_TAGS := optional")
			fmt.Fprintln(w, "LOCAL_PREBUILT_MODULE_FILE :=", m.outputFile.String())
			fmt.Fprintln(w, "LOCAL_MODULE_TARGET_ARCH :=", m.Arch().ArchType.String())
			fmt.Fprintln(w, "LOCAL_MODULE_PATH :=", "$(TARGET_ROOT_OUT)/bionic/" + m.pathInPartition)
			fmt.Fprintln(w, "LOCAL_INSTALLED_MODULE_STEM :=", m.stem)

			if len(m.properties.Symlinks) > 0 {
				symlink_dir_in_system := "$(TARGET_OUT)/" + m.pathInPartition + "/"
				symlink_dir_in_recovery := "$(TARGET_RECOVERY_ROOT_OUT)/system/" + m.pathInPartition + "/"
				symlink_target := "/bionic/" + m.pathInPartition + "/" + m.stem
				cmds := []string{}
				cmds = append(cmds, "$(hide) mkdir -p " + symlink_dir_in_system)
				cmds = append(cmds, "mkdir -p " + symlink_dir_in_recovery)
				for _, s := range m.properties.Symlinks {
					symlink := s + String(m.properties.Suffix)
					cmds = append(cmds, "ln -sf " + symlink_target + " " + symlink_dir_in_system +  symlink)
					cmds = append(cmds, "ln -sf " + symlink_target + " " + symlink_dir_in_recovery +  symlink)
				}
				fmt.Fprintln(w, "LOCAL_POST_INSTALL_CMD := " + strings.Join(cmds, " && "))
			}
			fmt.Fprintln(w, "include $(BUILD_PREBUILT)")
		},
	}
}

func bionicMountpointFactory() android.Module {
	m := &bionicMountpoint{}
        m.AddProperties(&m.properties)
	android.InitAndroidArchModule(m, android.DeviceSupported, android.MultilibBoth)
	return m
}

var Bool = proptools.Bool
var String = proptools.String
