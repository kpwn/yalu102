# yalu102

![Yalu logo](https://github.com/gzimbric/yalu102/blob/master/yalu102/images/yalugithub.png)

A "work in progress" iOS jailbreak for 64-bit devices created by [qwertyoruiopz](https://twitter.com/qwertyoruiopz) and [marcograssi](marcograss).

This version of yalu uses the same underlying code, however features a new ui and app icon.

This fork is generally updated from [kpwn's branch](https://github.com/kpwn/yalu102) every few hours. 

Compiled .ipa files are pushed every few hours/whenever a major update occurs.

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
| iPod touch (6G)  | iOS 10.0.0 -> iOS 10.2 |

### Planned Support:

In the near future, the jailbreak will support the following devices:

| Device | Version | 
|---------|----------|
| iPad Air 2| iOS 10.0.0 -> iOS 10.2 |
| iPad Mini 4 | iOS 10.0.0 -> iOS 10.2 |
| iPhone 7  | iOS 10.0.0 -> iOS 10.1.1 |

**Note, the iPhone 7 is only supported till iOS 10.1.1**
If you are already on iOS 10.2 with an iPhone 7, **stay there**. The actual exploit behind this still works, but the KPP bypass does not.

## Getting Started

1. `git clone` the repo.
2. Open the repo in Xcode
3. Change the bundle ID, as shown [here](https://www.reddit.com/r/sideloaded/wiki/how-to-sideload#wiki_changing_the_bundle_identifier_and_team)
4. Include the IOKit headers, and add them to your search path.
5. Run the project.

## Compiling

1. Click 'Product' in Xcode menu bar, from drop-down click 'Archive'
2. Right-click on the archive in Organizer, click 'Show in Finder'
3. Open Terminal and use the `cd` command to go to the directory the .archive file is located in
4. In Terminal, enter `xcodebuild -exportArchive -exportFormat ipa -archivePath <FILE_NAME>.xcarchive -exportPath ~/Desktop/<FILE_NAME>.ipa` replacing `<FILE_NAME>` with the archive name in Finder
5. Wait for command to complete, the .ipa file should be located on your desktop

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
| Beta 7  | [Link](https://lampserv.net/yalu.php) | 77057857E3AF6D3A920656432EC63299F0BC727A  |



## Contributing

Create a fork of the repository, make your changes and then create a pull request.
Please be sure to check if the pull request has been made before, before creating a new one. Note, any pull requests adding IOKit headers will be closed. Please respect copyright laws, and do not distribute / download IOKit headers from unofficial sources: they are bundled legally with macOS SDK
