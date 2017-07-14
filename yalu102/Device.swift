//
//  Device.swift
//
//  Created by AppleBetas on 2015-12-30.
//  Copyright © 2015 AppleBetas. All rights reserved.
//
//  https://gist.github.com/AppleBetas/e08cc34763500907fb2681facf1d948a
//

import Foundation

class Device {
    
    let deviceModel: String
    
    init(deviceModel: String) {
        self.deviceModel = deviceModel
    }
    
    convenience init() {
        self.init(deviceModel: Device.getDeviceModel())
    }
    
    class func getDeviceModel() -> String {
        var systemInfo = utsname()
        uname(&systemInfo)
        let machineMirror = Mirror(reflecting: systemInfo.machine)
        let identifier = machineMirror.children.reduce("") { identifier, element in
            guard let value = element.value as? Int8, value != 0 else { return identifier }
            return identifier + String(UnicodeScalar(UInt8(value)))
        }
        return identifier
    }
    
    func getDeviceName(extra: Bool) -> String {
        let platform = self.deviceModel
        let deviceNamesByCode = [
            // Simulators
            "x86_64": "64-bit Simulator",
            "i386": "32-bit Simulator",
            
            // iPod touches
            "iPod1,1": "iPod touch 1",
            "iPod2,1": "iPod touch 2",
            "iPod3,1": "iPod touch 3",
            "iPod4,1": "iPod touch 4",
            "iPod5,1": "iPod touch 5",
            "iPod7,1": "iPod touch 6",
            
            // iPads
            "iPad1,1": "iPad 1",
            "iPad2,1": extra ? "iPad 2 (Wi-Fi)" : "iPad 2",
            "iPad2,2": extra ? "iPad 2 (Wi-Fi/GSM)" : "iPad 2",
            "iPad2,3": extra ? "iPad 2 (Wi-Fi/CDMA)" : "iPad 2",
            "iPad2,4": extra ? "iPad 2 (Wi-Fi/Rev A)" : "iPad 2",
            "iPad3,1": extra ? "iPad 3 (Wi-Fi)" : "iPad 3",
            "iPad3,2": extra ? "iPad 3 (Wi-Fi/GSM/CDMA)" : "iPad 3",
            "iPad3,3": extra ? "iPad 3 (Wi-Fi/GSM)" : "iPad 3",
            "iPad3,4": extra ? "iPad 4 (Wi-Fi)" : "iPad 4",
            "iPad3,5": extra ? "iPad 4 (Wi-Fi/GSM)" : "iPad 4",
            "iPad3,6": extra ? "iPad 4 (Wi-Fi/GSM/CDMA)" : "iPad 4",
            "iPad4,1": extra ? "iPad Air (Wi-Fi)" : "iPad Air",
            "iPad4,2": extra ? "iPad Air (Wi-Fi/Cellular)" : "iPad Air",
            "iPad4,3": extra ? "iPad Air (Wi-Fi/China Cellular)" : "iPad Air",
            "iPad5,3": extra ? "iPad Air 2 (Wi-Fi)" : "iPad Air 2",
            "iPad5,4": extra ? "iPad Air 2 (Wi-Fi/Cellular)" : "iPad Air 2",
            "iPad2,5": extra ? "iPad mini 1 (Wi-Fi)" : "iPad mini 1",
            "iPad2,6": extra ? "iPad mini 1 (Wi-Fi/GSM/CDMA)" : "iPad mini 1",
            "iPad2,7": extra ? "iPad mini 1 (Wi-Fi/GSM)" : "iPad mini 1",
            "iPad4,4": extra ? "iPad mini 2 (Wi-Fi)" : "iPad mini 2",
            "iPad4,5": extra ? "iPad mini 2 (Wi-Fi/Cellular)" : "iPad mini 2",
            "iPad4,6": extra ? "iPad mini 2 (Wi-Fi/China Cellular)" : "iPad mini 2",
            "iPad4,7": extra ? "iPad mini 3 (Wi-Fi)" : "iPad mini 3",
            "iPad4,8": extra ? "iPad mini 3 (Wi-Fi/Cellular)" : "iPad mini 3",
            "iPad4,9": extra ? "iPad mini 3 (Wi-Fi/China Cellular)" : "iPad mini 3",
            "iPad5,1": extra ? "iPad mini 4 (Wi-Fi)" : "iPad mini 4",
            "iPad5,2": extra ? "iPad mini 4 (Wi-Fi/Cellular)" : "iPad mini 4",
            "iPad6,7": extra ? "iPad Pro (Wi-Fi, 12.9-inch)" : "iPad Pro",
            "iPad6,8": extra ? "iPad Pro (Wi-Fi/Cellular, 12.9-inch)" : "iPad Pro",
            "iPad6,3": extra ? "iPad Pro (Wi-Fi, 9.7-inch)" : "iPad Pro",
            "iPad6,4": extra ? "iPad Pro (Wi-Fi/Cellular, 9.7-inch)" : "iPad Pro",
            "iPad6,11": extra ? "iPad 5 (Wi-Fi, 9.7-inch)" : "iPad 5",
            "iPad6,12": extra ? "iPad 5 (Wi-Fi/Cellular, 9.7-inch)" : "iPad 5",
            
            // iPhones
            "iPhone1,1": "iPhone 1",
            "iPhone1,2": "iPhone 3G",
            "iPhone2,1": "iPhone 3GS",
            "iPhone3,1": extra ? "iPhone 4 (GSM)" : "iPhone 4",
            "iPhone3,2": extra ? "iPhone 4 (GSM/2012)" : "iPhone 4",
            "iPhone3,3": extra ? "iPhone 4 (CDMA)" : "iPhone 4",
            "iPhone4,1": "iPhone 4s",
            "iPhone5,1": "iPhone 5",
            "iPhone5,2": "iPhone 5",
            "iPhone5,3": "iPhone 5c",
            "iPhone5,4": "iPhone 5c",
            "iPhone6,1": "iPhone 5s",
            "iPhone6,2": "iPhone 5s",
            "iPhone7,2": "iPhone 6",
            "iPhone7,1": "iPhone 6 Plus",
            "iPhone8,1": "iPhone 6s",
            "iPhone8,2": "iPhone 6s Plus",
            "iPhone8,4": "iPhone SE",
            "iPhone9,1": "iPhone 7",
            "iPhone9,2": "iPhone 7 Plus",
            "iPhone9,3": "iPhone 7",
            "iPhone9,4": "iPhone 7 Plus",
            
            // Apple TVs
            "AppleTV2,1": "Apple TV 2",
            "AppleTV3,1": "Apple TV 3",
            "AppleTV3,2": extra ? "Apple TV 3 (Rev A)" : "Apple TV 3",
            "AppleTV5,3": "Apple TV 4",
            
            // Apple Watches
            "Watch1,1": extra ? "Apple Watch (38mm)" : "Apple Watch",
            "Watch1,2": extra ? "Apple Watch (42mm)" : "Apple Watch",
            "Watch2,6": extra ? "Apple Watch Series 1 (38mm)" : "Apple Watch Series 1",
            "Watch2,7": extra ? "Apple Watch Series 1 (42mm)" : "Apple Watch Series 1",
            "Watch2,3": extra ? "Apple Watch Series 2 (38mm)" : "Apple Watch Series 2",
            "Watch2,4": extra ? "Apple Watch Series 2 (42mm)" : "Apple Watch Series 2",
            ]
        
        if let deviceName = deviceNamesByCode[platform] {
            return deviceName
        }
        
        return self.getDeviceFamily()
    }
    
    func getDeviceFamily() -> String {
        let platform = self.deviceModel
        let deviceFamilyByCodeStart = [
            "iPhone": "iPhone",
            "iPod": "iPod",
            "iPad": "iPad",
            "AppleTV": "Apple TV",
            "Watch": "Apple Watch",
            "Mac": "PowerMac",
            "PowerBook": "PowerBook",
            "iMac": "iMac",
            "Macmini": "Mac mini",
            "MacPro": "Mac Pro",
            "MacBook Air": "MacBook Air",
            "MacBook Pro": "MacBook Pro",
            "MacBook": "MacBook",
            "RackMac": "Xserve",
            "Xserve": "Xserve"
        ]
        
        for (deviceFamilyID, deviceFamily) in deviceFamilyByCodeStart {
            if platform.hasPrefix(deviceFamilyID) {
                return deviceFamily
            }
        }
        
        return platform
    }
}
