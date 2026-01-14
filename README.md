# About JUCE_ARA

This repository is an experimental fork of the [JUCE develop branch](https://github.com/juce-framework/JUCE), originally drafting additions by [Celemony](https://www.celemony.com) and [SoundRadix](https://www.soundradix.com) that enabled JUCE to generate [ARA plug-ins](https://github.com/Celemony/ARA_SDK) in the VST3 or AudioUnit format. With JUCE 7, these additions have been reviewed, refactored and properly integrated into JUCE main line.

The develop branch of this fork has been updated to JUCE 7 accordingly. It will now merely be used for adding experimental implementations of draft API from the ARA SDK which will come to JUCE mainline only after they have been finalized in the ARA SDK. Accordingly, the original JUCE repository should be used for developing actual products.

There also are two legacy branches for JUCE 6 users which will not be maintained further: JUCE_6_develop and JUCE_6_condensed. JUCE_6_develop was used for active development and contains all ARA-related changes including some ARA draft features still under development, whereas JUCE_6_condensed is based on JUCE 6 release code and uses only released ARA APIs. JUCE_6_condensed also features a heavily rebased change history for easier merging into JUCE mainline. This makes it also easy to rebase onto any custom fork of JUCE 6 that might already be in use.

Note that Celemony is not endorsing the use of JUCE by providing this fork. The ARA integration in JUCE is merely an adapter, it does not provide any features that would be relevant when using ARA with a different framework. Consequently, the decision whether or not to use JUCE for any given project should be made independently of this fork.

For feedback and questions, please contact Celemony via [ara@celemony.com](mailto:ara@celemony.com?Subject=JUCE%20ARA%20feedback).


# Original JUCE documentation

![alt text](https://assets.juce.com/juce/JUCE_banner_github.png "JUCE")

JUCE is an open-source cross-platform C++ application framework for creating
desktop and mobile applications, including VST, VST3, AU, AUv3, AAX and LV2
audio plug-ins and plug-in hosts. JUCE can be easily integrated with existing
projects via CMake, or can be used as a project generation tool via the
[Projucer](#the-projucer), which supports exporting projects for Xcode (macOS
and iOS), Visual Studio, Android Studio, and Linux Makefiles as well as
containing a source code editor.

## Getting Started

The JUCE repository contains a
[master](https://github.com/juce-framework/JUCE/tree/master) and
[develop](https://github.com/juce-framework/JUCE/tree/develop) branch. The
develop branch contains the latest bug fixes and features and is periodically
merged into the master branch in stable [tagged
releases](https://github.com/juce-framework/JUCE/releases) (the latest release
containing pre-built binaries can be also downloaded from the [JUCE
website](https://juce.com/get-juce)).

JUCE projects can be managed with either the Projucer (JUCE's own
project-configuration tool) or with CMake.

### The Projucer

The repository doesn't contain a pre-built Projucer so you will need to build it
for your platform - Xcode, Visual Studio and Linux Makefile projects are located
in [extras/Projucer/Builds](/extras/Projucer/Builds) (the minimum system
requirements are listed in the [minimum system
requirements](#minimum-system-requirements) section below). The Projucer can
then be used to create new JUCE projects, view tutorials and run examples. It is
also possible to include the JUCE modules source code in an existing project
directly, or build them into a static or dynamic library which can be linked
into a project.

For further help getting started, please refer to the JUCE
[documentation](https://juce.com/learn/documentation) and
[tutorials](https://juce.com/learn/tutorials).

### CMake

Version 3.22 or higher is required. To use CMake, you will need to install it,
either from your system package manager or from the [official download
page](https://cmake.org/download/). For comprehensive documentation on JUCE's
CMake API, see the [JUCE CMake documentation](/docs/CMake%20API.md). For
examples which may be useful as starting points for new CMake projects, see the
[CMake examples directory](/examples/CMake).

#### Building Examples

To use CMake to build the examples and extras bundled with JUCE, simply clone
JUCE and then run the following commands, replacing "DemoRunner" with the name
of the target you wish to build.

    cd /path/to/JUCE
    cmake . -B cmake-build -DJUCE_BUILD_EXAMPLES=ON -DJUCE_BUILD_EXTRAS=ON
    cmake --build cmake-build --target DemoRunner

## Minimum System Requirements

#### Building JUCE Projects

- __C++ Standard__: 17
- __macOS/iOS__: Xcode 12.4 (Intel macOS 10.15.4, Apple Silicon macOS 11.0)
- __Windows__: Visual Studio 2019 (Windows 10)
- __Linux__: g++ 7.0 or Clang 6.0 (for a full list of dependencies, see
[here](/docs/Linux%20Dependencies.md)).
- __Android__: Android Studio (NDK 26) on Windows, macOS or Linux

#### Deployment Targets

- __macOS__: macOS 10.11 (x86_64, Arm64)
- __Windows__: Windows 10 (x86_64, x86, Arm64, Arm64EC)
- __Linux__: Mainstream Linux distributions (x86_64, Arm64/aarch64, (32 bit Arm systems like armv7 should work but are not regularly tested))
- __iOS__: iOS 12 (Arm64, Arm64e, x86_64 (Simulator))
- __Android__: Android 7 - Nougat (API Level 24) (arm64-v8a, armeabi-v7a, x86_64, x86)

## Contributing

Please see our [contribution guidelines](.github/contributing.md).

## Licensing

See [LICENSE.md](LICENSE.md) for licensing and dependency information.

## AAX Plug-Ins

AAX plug-ins need to be digitally signed using PACE Anti-Piracy's signing tools
before they will run in commercially available versions of Pro Tools. These
tools are provided free of charge by Avid. Before obtaining the signing tools,
you will need to use a special build of Pro Tools, called Pro Tools Developer,
to test your unsigned plug-ins. The steps to obtain Pro Tools Developer are:

1. Sign up as an AAX Developer [here](https://developer.avid.com/aax/).
2. Request a Pro Tools Developer Bundle activation code by sending an email to
   [devauth@avid.com](mailto:devauth@avid.com).
3. Download the latest Pro Tools Developer build from your Avid Developer
   account.

When your plug-ins have been tested and debugged in Pro Tools Developer, and you
are ready to digitally sign them, please send an email to
[audiosdk@avid.com](mailto:audiosdk@avid.com) with the subject "PACE Eden
Signing Tools Request". You need to include an overview of each plug-in along
with a screen recording showing the plug-in running in Pro Tools Developer, with
audio if possible.

Please also include the following information:

- Company name
- Admin full name
- Telephone number

Once the request is submitted, PACE Anti-Piracy will contact you directly with
information about signing your plug-ins. When the plug-ins have been signed, you
are free to sell and distribute them. If you are interested in selling your
plug-ins on the Avid Marketplace, please send an email to
[audiosdk@avid.com](mailto:audiosdk@avid.com).
