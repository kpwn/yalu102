# yalu102

![Yalu logo](https://github.com/kpwn/yalu102/blob/master/yalu102/Assets.xcassets/AppIcon.appiconset/AppIcon60x60@3x.png?raw=true)

A "work in progress" iOS jailbreak for 64-bit devices created by [qwertyoruiopz](https://twitter.com/qwertyoruiopz) and [marcograssi](marcograss).

Please use the "Issues" tab for **code related** issues only. If you need support please search on [/r/jailbreak](https://reddit.com/r/jailbreak) before posting a question there.

## Supported Devices and iOS versions

| Device | Version |
|---------|----------|
| iPad Pro  | iOS 10.0.0 -> iOS 10.2 |
| iPhone 6S  | iOS 10.0.0 -> iOS 10.2 |
| iPhone SE  | iOS 10.0.0 -> iOS 10.2 |
| iPhone 5S  | iOS 10.0.0 -> iOS 10.2 |
| iPad Air| iOS 10.0.0 -> iOS 10.2 |
| iPad Mini 2| iOS 10.0.0 -> iOS 10.2 |
| iPhone 6  | iOS 10.0.0 -> iOS 10.2 |
| iPad Mini 3| iOS 10.0.0 -> iOS 10.2 |
| iPad Air 2| iOS 10.0.0 -> iOS 10.2 |
| iPad Mini 4 | iOS 10.0.0 -> iOS 10.2 |
| iPod touch (6G)  | iOS 10.0.0 -> iOS 10.2 |

### Planned Support:

In the near future, the jailbreak will support the following devices:

| Device | Version |
|---------|----------|
| iPhone 7  | iOS 10.0.0 -> iOS 10.1.1 |

**Note, the iPhone 7 is only supported till iOS 10.1.1**
If you are already on iOS 10.2 with an iPhone 7, **stay there**. The actual exploit behind this still works, but the KPP bypass does not.

## Compiling:

1. `git clone` the repo.
2. Open the repo in Xcode
3. Change the bundle ID, as shown [here](https://www.reddit.com/r/sideloaded/wiki/how-to-sideload#wiki_changing_the_bundle_identifier_and_team)
4. Include the IOKit headers, and add them to your search path.
5. Run the project.

## Warnings

This jailbreak is a work in progress. Some things do not work, but most things do.

Do not install things that are untested.

**AppSync and other unsupported and untested software will probably throw your device into a bootloop or do other bad things.** Do not open an issue complaining that your device has been bootlooped because you installed other software. You have been warned.

## Installing

> DO NOT DOWNLOAD THIS SOFTWARE FROM OTHER SOURCES OTHER THAN THESE LINKS UNDER ANY CIRCUMSTANCE. IT IS VERY EASY TO BACKDOOR THIS SORT OF SOFTWARE TO CONTAIN MALWARE. PLEASE BE EXTREMELY CAREFUL. THESE MIRRORS ARE TRUSTED, BUT STILL CHECK THE SHA1.

* Download the pre-compiled version from the table below.
* [Check the SHA1 hash](http://onlinemd5.com) of the downloaded file (optional but recommended).
* Install using [Cydia Impactor](http://www.cydiaimpactor.com/).
* Open the application and follow instructions.


| Version | Download | SHA1 |
|---------|----------|------|
| Beta 7  | [Link](https://yalu.qwertyoruiop.com/yalu102_beta7.ipa) | 4afa99d4b568aa8cbb9ac61fddd584111fed79c5  |
| Beta 6  | [Link](https://yalu.qwertyoruiop.com/yalu102_beta6.ipa) | 0130ebe60c97e2013a4b849b7d9bc321d749f304  |
| Beta 5  | [Link](https://yalu.qwertyoruiop.com/yalu102_beta5.ipa) | f8eb6cd37054a9d25b818e3bddd13bfedbf72df1  |
| Beta 4  | [Link](https://yalu.qwertyoruiop.com/yalu102_beta4.ipa) | f8270e59d7d7267613ffa63217b91fea425eec36  |
| Beta 3  | [Link](https://yalu.qwertyoruiop.com/yalu102_beta3.ipa) | b2e0bdd31566f876d67cba036b5d29aef7ff257d  |
| Beta 2  | [Link](https://yalu.qwertyoruiop.com/yalu102_beta.ipa) | 4fddad7cca8aa0c0a6579c1d63d00917f15efc86  |
| Beta 1  | [Link](https://yalu.qwertyoruiop.com/yalu102_alpha.ipa) | 2fe14f1c1e1a0d26203bbb123f6747a978dd2b4f  |

## Contributing

Create a fork of the repository, make your changes and then create a pull request.
Please be sure to check if the pull request has been made before, before creating a new one. Note, any pull requests adding IOKit headers will be closed. Please respect copyright laws, and do not distribute / download IOKit headers from unofficial sources: they are bundled legally with macOS SDK

