# bootburn (flashtools plugin)

Flashtools plugin to use **Nvidia** `bootburn` tool to flash targets. The plugin
is intended as a wrapper for usage of bootburn shell script providing some
options to define which target and data should be flashed and changing basic
configuration parts which are used for generating image data to be flashed onto
target.

## Contents

- [Overview](#overview)
- [Usage](#usage)
- [How to Add Custom Board Configuration](#how-to-add-custom-board-configuration)
- [Mett Plugin Configuration](#mett-plugin-configuration)
- [NVIDIA PCT Config Replacement](#nvidia-pct-config-replacement)

## Overview

![bootburn overview](./docs/images/bootburn_overview.png)

Note: the foundation is extracted without execution scripts.
That means it contains variables like `<PDK_TOP>`.
Those will be replaced by the Mett bootburn plugin.

| Component | Description |
| --- | --- |
| NVIDIA bootburn | Flashtool from NVIDIA
| mett bootburn | Mett Flashtool Plugin `bootburn`.
| PCT Config | PCT Config from `<foundation>/virtualization/pct/<board>`.
| config.git | Target config + subsystem config needed for bootburn.

NVIDIA Bootburn copies the configuration to a new location:

```text
<foundation>/virtualization/hypervisor/t19x/configs/t194ref-release/pct/e3550b03-t194a
```

## Usage

Clean and Flash using bootburn.
Generated images will be removed after flashing
```bash
tools/deploy.py --options "aurix-device /dev/ttyUSB3"
```

Create BSP Images
```bash
tools/deploy.py --options "launch create-images"
```

Flash BSP Images (This will reuse the previously generated images and not
generate the new images)
```bash
tools/deploy.py --options "aurix-device /dev/ttyUSB3" -o "launch flash-images"
```

Make binding. Make binding compile the dts and generate the security images
(e.g tos.img)
```bash
tools/deploy.py --options "launch make-bind"
```

Bootburn plugin-specific command line options:

```text
  aurix-device          [bootburn] Serial device/port of aurix serial for
                        automatic flashing
  bootburn-args         [bootburn] EXPERIMENTAL: Additional bootburn arguments
                        added to call
  launch                To launch/start the spcific steps for nvidia flashing
	                procedure. The steps can be make-bind, flash-images &
                        create-images
```

## How to Add Custom Board Configuration

### Custom PCT

***Note: This point is currently under discussion with NVIDIA (NBP-438).***

NVIDIA PCT files are located here in the foundation.
Both boards have their own set of files.

```text
<foundation-dir>/virtualization/pct
- e3550b03-t194a
- e3550b03-t194b
```

In order to create your own PCT, you need to:

1. Copy an existing board your basing on (e.g. e3550b03-t194a).

    Ideally not into the foundation folder, but a separate location.
    For example:

    ```text
    virtualization/pct/<nv-boards> --> config.git/bootburn/pct/<your-boards>
    virtualization/build_configs/<nv-boards> --> config.git/bootburn/build_configs/<your-boards>
    ```

    You need to do `ln -s virtualization/pct/<your-boards> config.git/bootburn/pct/<your-boards>`.
    We could change `PCT_PATH := $(BASE)/pct/$(BOARD)/` in Makefile.bind to avoid that.

    Copy some generic files. (tbd: use only ln)

    ```text
    virtualization/pct/make --> config.git/bootburn/pct/make
    virtualization/pct/dtb_include --> config.git/bootburn/pct/dtb_include
    ```

    Now you should be able to call:

    ```text
    make -f Makefile.bind BOARD=<your-board> PCT=qnx
    ```

    Note: Makefile.bind makes your `<foundation>` repository dirty by creating
    files and folders everywhere.

2. Add the new PCT to bootburn_helper

    TBD

```text
export PATH=<workdir>/bp4ad/repos/nv-release-bins/toolchains/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin:${PATH} ; export QNX_HOST=<workdir>/bp4ad/install/qnx-sdp_700/5.1.6/host/linux/x86_64; export QNX_TARGET=<workdir>/bp4ad/install/qnx-sdp_700/5.1.6/target/qnx7;  ./bootburn.sh -b neckar-t194a -B qspi
```

## Mett Plugin Configuration

The table below lists the variables used in the bootburn plugin and where they are defined.

| variable                           | defined in .yaml                                |
|------------------------------------|-------------------------------------------------|
| generated.nvidia.foundation_dir    | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| generated.nvidia.qnx_host_dir      | config/portfolio/qnx/sdp-x_x_x.yaml             |
| generated.nvidia.qnx_target_dir    | config/portfolio/qnx/sdp-x_x_x.yaml             |
| options.stage.deploy.bootburn-args | commandline options                             |
| generated.working_dir              | generated in current dir (constant.py)          |
| pct_name                           | config/portfolio/nvidia/qnx-x_x_x_x.yaml        |
| nv-aurix.target-reset-wait         | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv-aurix.acknowledgement           | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv-aurix.version-cmd               | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv-aurix.target-reset-cmd          | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv_foundation.out_dir              | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv_bootburn.dir                    | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv_bootburn.qb_flash_dir           | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv_bootburn.script                 | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv-aurix.reset-cmd                 | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv-aurix.recovery-cmd              | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv-aurix.recovery-device-name      | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv-aurix.recovery-device-wait      | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv_foundation.make                 | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv_foundation.version              | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv_foundation.version_file         | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| nv_foundation.cc_file              | config/portfolio/nvidia/foundation-x_x_x_x.yaml |
| toolchain.cc                       | config/portfolio/nvidia/gcc-x_x_x-linaro.yaml   |
| toolchain.path                     | config/portfolio/nvidia/gcc-x_x_x-linaro.yaml   |
| name                               | config/flashtools/bootburn/e3550b03-t194X.yaml  |
| board-arch                         | config/flashtools/bootburn/e3550b03-t194X.yaml  |
| board-name                         | config/flashtools/bootburn/e3550b03-t194X.yaml  |
| board-switch                       | config/flashtools/bootburn/e3550b03-t194X.yaml  |


## NVIDIA PCT Config Replacement

The Bootburn plugin can replace variables like `<PDK_TOP>` in NVIDIA configuration files. The plugin expects the following structure:

```yaml
<target-name>:
  name: <human-readable-name>
  target-name: <target-name>
  # ...
  pct: # common replacements
    modifiers: #
    qnx: # QNX-only
      modifiers: #
      files: #
  arch: # replacements depending on architecture (pegasus vs neckar)
    modifiers: #
    files: #
    pct:
      qnx: # QNX-only
        modifiers: #
        files: #
  boards: # replacements depending on board (a vs b)
    - board-name: e3550b03-t194a
      modifiers: #
      files: #
      pct:
        qnx: # QNX-only
        modifiers: #
        files: #
    - board-name: e3550b03-t194b
      modifiers: #
      files: #
      pct:
        qnx: # QNX-only
        modifiers: #
        files: #
```

The sections `pct` and `arch` are applied to both boards and the individual
`boards` section only to the respective board.
Move files and modifiers depending on your needs.

### Section `modifiers`

Contains a list of named modifiers to use for replacement later in the `files`
section.

*modifier*.`task`: Possible values are `replace` or `set-path`.

replace: replace search pattern with *modifier*.`replace`.

set-path: replace full value with *modifier*.`set-path`.

*modifier*.`pattern`: Search pattern for replacement.

*modifier*.`replace`: String to replace a pattern in the original value.

*modifier*.`set-path`: String to use replace original value.

Examples:

```yaml
modifiers:
  pdk-top-bins:
    task: replace
    pattern: <PDK_TOP>
    replace: '{{ config.get("generated.repos.nv-release-bins.path") }}'
  kernel-dtb:
    task: set-path
    path:
      - '{{ config.get("generated.deploy_dir") }}'
      - boot/devicetree-Image-tegra194-e3550-0001-b03-A-linux-driveav-vm1.dtb
```

### Section `files`

Contains a list of files to process.

`name`: Natural name.

`path`: File path as an array.

`modifications`: List of modifications.

*modifications*.`key`: Key in NVIDIA config file.

*modifications*.`expected`: Original value to cross-check.

*modifications*.`modifiers`: List of modifiers to apply.

Example:

```yaml
- name: quickboot qspi qnx configuration - pct (qnx)
  path:
    - '{{ config.get("generated.nvidia.foundation_dir") }}'
    - <<bootburn-dir>>
    - quickboot_qspi_qnx.cfg
  modifications:
    - key: partition[name=bpmp-fw]filename
      expected: <PDK_TOP>/drive-<TARGET_BOARD>-foundation/firmwares/bin/t186/bpmp/bpmp.bin
      modifiers:
        - target-board
        - pdk-top-bins
```

### Available Variables

There are two types of variables available.

1. Access to ``{{ config.get("...") }}``, just like in all other Mett
   configurations.
2. Special variables with ``<<...>>`` notation.

| Special Variable | Content |
| --- | --- |
| `<<bootburn-dir>>` | *subsystem*.`nv_foundation.out_dir` + *board*.`board-name` |
| `<<bootburn-arch-dir>>` | *subsystem*.`nv_bootburn.dir` |
| `<<quickboot-flash-dir>>` | *subsystem*.`nv_bootburn.dir` + *board*.`qb_flash_dir_ext` |
