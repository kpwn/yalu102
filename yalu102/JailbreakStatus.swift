//
//  JailbreakStatus.swift
//  yalu102
//
//  Created by AppleBetas (ported by mullak99)
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

import UIKit

enum JailbreakStatus: Int32 {
    case jailbroken = 0, internalError = 1, unsupported = 2, unsupportedYet = 3, ok = 42, unknown = -1, alreadyJailbroken = 99
    
    static func status(from numericValue: Int32) -> JailbreakStatus {
        if let status = JailbreakStatus(rawValue: numericValue) {
            return status
        }
        return .unknown
    }
    
    var success: Bool {
        return self == .jailbroken || self == .ok
    }
    
    var ringColour: UIColor {
        switch self {
        case .jailbroken:
            return .green
        case .ok:
            return .orange
        default:
            return .red
        }
    }
    
    var shouldShowAlert: Bool {
        return self != .jailbroken
    }
    
    var shouldAlertHaveExitButton: Bool {
        return self != .ok
    }
    
    var progressState: ProgressState {
        if success {
            return ProgressState(text: "Done!", image: UIImage(named: "success"), spinnerState: .full, overrideRingColour: self.ringColour)
        }
        return ProgressState(text: "Error", image: UIImage(named: "fail"), spinnerState: .full, overrideRingColour: .red)
    }
    
    var alertTitle: String {
        if success {
            return "Jailbroken"
        }
        return "An Error Occurred"
    }
    
    var alertMessage: String {
        switch self {
        case .jailbroken:
            return "Your device has been successfully jailbroken!"
        case .internalError:
            return "An internal error occurred while attempting to jailbreak your device."
        case .unsupported:
            return "Your device is not supported."
        case .unsupportedYet:
            return "Your device is not yet supported."
        case .ok:
            return "Your device has been jailbroken, but may not work correctly."
        case .unknown:
            return "An error occurred while attempting to jailbreak your device. Please reboot."
        case .alreadyJailbroken:
            return "Your device is already jailbroken."
        }
    }
}
