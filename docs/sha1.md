# How To Verify Your Install

The SHA1 checksums are hashes developed by a Secure Hash Algorithm. ELI5: Every file has a specific SHA1 checksum that is changed every time the file is changed. This is to make sure your download isn't corrupt or contains harmful code.

## macOS and Linux (Unix based OS)

* Run the following command into Terminal:

`openssl sha1 [path to IPA]`

* If it matches up with the below checksums then it should be okay.

## Windows

* Run the following command into Command Prompt:

`CertUtil -hashfile [path to IPA] SHA1`

* If it matches up with the below checksums then it should be okay.

## SHA1 Checksums

Version | Download | SHA1
:----------- |:-------------:| -----------:
yalu103 - Alpha | [Link](https://yalu.qwertyoruiop.com/yalu102_alpha.ipa) | 2fe14f1c1e1a0d26203bbb123f6747a978dd2b4f
YaluB3 | [Link](https://yalu.qwertyoruiop.com/mach_portal+yalu-b3.ipa) | f34ba1bebf02b265a871158e778596ee2ff1547a69fdb687d911897b7b7b22c5