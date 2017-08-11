//
//  ViewController.swift
//  yalu102
//
//  Created by AppleBetas (ported by mullak99)
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

import UIKit

class ViewController: UIViewController, DrawerToggleViewDelegate {
    @IBOutlet weak var goButton: SexyFillButton!
    @IBOutlet weak var deviceLabel: UILabel!
    @IBOutlet weak var progressContainerView: UIVisualEffectView!
    @IBOutlet weak var progressView: ProgressView!
    @IBOutlet weak var drawerToggleView: DrawerToggleView!
    @IBOutlet weak var mainContentView: UIView!
    @IBOutlet weak var menuOpenedConstraint: NSLayoutConstraint!
    @IBOutlet weak var menuClosedConstraint: NSLayoutConstraint!
    @IBOutlet weak var menuDarkeningView: UIView!
    @IBOutlet weak var substrateEnabledSwitch: UISwitch!
    var hasStarted = false
    var systemVersion = UIDevice.current.systemVersion
    

    override func viewDidLoad() {
        super.viewDidLoad()
        progressContainerView.effect = nil
        progressView.alpha = 0
        progressContainerView.isHidden = true
        substrateEnable = (substrateEnabledSwitch.isOn ? 1 : 0);
        progressView.updateProgressState(with: ProgressState(text: "Working on it…", image: nil, spinnerState: .none, overrideRingColour: nil), animated: false)
        drawerToggleView.delegate = self
        menuDarkeningView.addGestureRecognizer(UITapGestureRecognizer(target: self, action: #selector(ViewController.menuDarkeningViewTapped)))
        loadDeviceData()
        print("jailed (unreliable result):", Bool(isJailbroken() as NSNumber))
        print("substrate:", (Bool(substrateEnable as NSNumber) ? "enabled" : "disabled"));
    }
    
    override var preferredStatusBarStyle: UIStatusBarStyle {
        return .lightContent
    }

    override func viewWillTransition(to size: CGSize, with coordinator: UIViewControllerTransitionCoordinator) {
        super.viewWillTransition(to: size, with: coordinator)
        // Hide menu on rotation/view size change
        if drawerToggleView.isOpen {
            setDrawer(opened: false)
        }
    }
    
    @IBAction func substrateEnabled(_ sender: Any) {
        substrateEnable = (substrateEnabledSwitch.isOn ? 1 : 0);
        print("substrate:", (Bool(substrateEnable as NSNumber) ? "enabled" : "disabled"));
    }
    
    @IBAction func bang(_ sender: UIButton) {
        doJailbreak(sender)
    }
    
    func doJailbreak(_ sender: Any) {
        if !hasStarted {
            hasStarted = true
            progressContainerView.effect = nil
            progressView.alpha = 0
            progressContainerView.alpha = 1
            progressContainerView.isHidden = false
            progressView.updateProgressState(with: ProgressState(text: "Working on it…", image: nil, spinnerState: .spinning, overrideRingColour: nil), animated: true)
            UIView.animate(withDuration: 0.25, animations: {
                self.progressContainerView.effect = UIBlurEffect(style: .dark)
                self.progressView.alpha = 1
            })
            
            OperationQueue().addOperation {
                let result = unjail()
                OperationQueue.main.addOperation {
                    self.handle(result: JailbreakStatus.status(from: result))
                }
            }
        }
    }
    
    private func handle(result: JailbreakStatus) {
        self.progressView.updateProgressState(with: result.progressState, animated: true)
        if result.shouldShowAlert {
            let alert = UIAlertController(title: result.alertTitle, message: result.alertMessage, preferredStyle: .alert)
            if result.shouldAlertHaveExitButton {
                alert.addAction(UIAlertAction(title: "Exit", style: .default, handler: { _ in
                    UIApplication.shared.performGracefulExit()
                }))
            }
            Timer.scheduledTimer(withTimeInterval: 0.5, repeats: false, block: { _ in
                OperationQueue.main.addOperation {
                    self.present(alert, animated: true, completion: nil)
                }
            })
        }
    }
    
    private func loadDeviceData() {
        let deviceName = Device().getDeviceName(extra: false)
        deviceLabel.text = "\n\(deviceName) (iOS \(systemVersion))"
        //goButton.isEnabled = supported
        //goButton.isEnabled = Bool(isJailbroken() as NSNumber)
    }
    
    func setDrawer(opened open: Bool) {
        menuOpenedConstraint.isActive = open
        menuClosedConstraint.isActive = !open
        menuDarkeningView.isUserInteractionEnabled = open
        UIView.animate(withDuration: 0.25, delay: 0, usingSpringWithDamping: 0.9, initialSpringVelocity: 1, animations: {
            self.drawerToggleView.isOpen = open
            self.menuDarkeningView.alpha = open ? 1 : 0
            self.mainContentView.alpha = open ? 0.6 : 1
            self.mainContentView.transform = open ? CGAffineTransform.identity.scaledBy(x: 0.85, y: 0.85) : .identity
            self.view.layoutIfNeeded()
        }, completion: nil)
    }

    func menuDarkeningViewTapped() {
        setDrawer(opened: false)
    }
    
    // MARK: - Drawer Toggle View Delegate
    
    func drawerToggleViewTapped(_ view: DrawerToggleView) {
        setDrawer(opened: !view.isOpen)
    }
    
}

