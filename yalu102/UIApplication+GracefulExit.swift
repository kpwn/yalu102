//
//  UIApplication+GracefulExit.swift
//
//  Created by AppleBetas on 2017-05-28.
//

import UIKit

extension UIApplication {
    
    /// Exit the application to the home screen and then terminate it.
    func performGracefulExit() {
        let suspendSelector = Selector("suspend") // swift can't hide warnings ffs
        if self.responds(to: suspendSelector) {
            Timer.scheduledTimer(withTimeInterval: 1, repeats: false, block: { _ in
                exit(0)
            })
            self.perform(suspendSelector)
        } else {
            exit(0)
        }
    }
    
}
