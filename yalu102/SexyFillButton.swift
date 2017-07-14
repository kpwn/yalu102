//
//  SexyFillButton.swift
//  yalu102
//
//  Created by AppleBetas (ported by mullak99)
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

import UIKit

class SexyFillButton: UIButton {

    override func didMoveToWindow() {
        super.didMoveToWindow()
        layer.borderWidth = 1
        layer.masksToBounds = true
        setColours()
        doCircleRadius()
        doPadding()
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        doCircleRadius()
    }
    
    override var isHighlighted: Bool {
        didSet {
            UIView.animate(withDuration: 0.15, animations: {
                self.alpha = self.isHighlighted ? 0.8 : 1
                self.transform = self.isHighlighted ? CGAffineTransform.identity.scaledBy(x: 0.9, y: 0.9) : .identity
            })
        }
    }
    
    override var isEnabled: Bool {
        didSet { self.setColours() }
    }
    
    override var tintColor: UIColor! {
        didSet { setColours() }
    }
    
    var textPadding: Float = 60 {
        didSet { doPadding() }
    }
    
    private func doPadding() {
        let padding = CGFloat(textPadding)
        contentEdgeInsets = UIEdgeInsets(top: padding, left: padding, bottom: padding, right: padding)
    }
    
    private func setColours() {
        layer.borderColor = (isEnabled ? tintColor : UIColor.darkGray)?.cgColor
        setTitleColor(isEnabled ? .white : .lightGray, for: [.normal])
    }
    
    private func doCircleRadius() {
        layer.cornerRadius = min(bounds.size.width, bounds.size.height) / 2
    }

}
