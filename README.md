# yalu102

![Yalu logo](https://github.com/kpwn/yalu102/blob/master/yalu102/Assets.xcassets/AppIcon.appiconset/AppIcon60x60@3x.png?raw=true)

A "work in progress" iOS jailbreak for 64-bit devices created by [qwertyoruiopz](https://twitter.com/qwertyoruiopz) and [marcograssi](marcograss).

Please use the "Issues" tab for **code related** issues only. If you need support please search on [/r/jailbreak](https://reddit.com/r/jailbreak) before posting a question there.

## Supported Devices

* iPhone SE
* iPhone 6S
* iPhone 6S+
* iPad Pro

## Supported iOS Versions

* 10.0.x
* 10.1.x
* 10.2

## Compiling:

1. `git clone` the repo.
2. Open the repo in Xcode
3. Change the bundle ID, as shown [here](https://www.reddit.com/r/sideloaded/wiki/how-to-sideload#wiki_changing_the_bundle_identifier_and_team)
4. Download the IOKit headers, and add them to your search path, noted in section below.
5. Run the project.

### Fixing the IOKit error:

1. Download the zip of this project https://github.com/kennytm/iphone-private-frameworks/tree/master/IOKit
2. Go to your target’s Build Settings. In the search bar write “headers”. In the results you’ll see a section called Search Paths. Change **“Always Search User Paths”** to **YES**
3. Add the path where you placed your headers in **“User Header Search Paths”**

**Make sure to put the folder that contains the IOKit folder, and not the IOKit folder itself.**
(steps from issue #71, thanks @4taras4)

## Installing

> DO NOT DOWNLOAD THIS SOFTWARE FROM OTHER SOURCES OTHER THAN THESE LINKS UNDER ANY CIRCUMSTANCE. IT IS VERY EASY TO BACKDOOR THIS SORT OF SOFTWARE TO CONTAIN MALWARE. PLEASE BE EXTREMELY CAREFUL. THESE MIRRORS ARE TRUSTED, BUT STILL CHECK THE SHA1.

* Download the pre-compiled version from the table below.
* [Check the SHA1 hash](http://onlinemd5.com) of the downloaded file (optional but recommended).
* Install using [Cydia Impactor](http://www.cydiaimpactor.com/).
* Open the application and follow instructions.
    

| Version | Download | SHA1 |
|---------|----------|------|
| Alpha  | [Link](https://yalu.qwertyoruiop.com/yalu102_alpha.ipa) | 2FE14F1C1E1A0D26203BBB123F6747A978DD2B4F  |

## Contributing

Create a fork of the repository, make your changes and then create a pull request.
Please be sure to check if the pull request has been made before, before creating a new one. Note, any pull requests adding IOKit headers will be closed. Please respect copyright laws.

