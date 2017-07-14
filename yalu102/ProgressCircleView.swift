//
//  ProgressCircleView.swift
//  yalu102
//
//  Created by AppleBetas (ported by mullak99)
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

import UIKit

class ProgressCircleView: UIView {
    static let defaultRingColour = UIColor(white: 0.9, alpha: 0.5)
    private var loadingCircle: CAShapeLayer?, imageView: UIImageView!, oldImageView: UIImageView!
    private var imageViewCenterOffsetConstraint: NSLayoutConstraint!
    private var _lastSpinnerState = ProgressState.SpinnerState.none, _lastRingColour = ProgressCircleView.defaultRingColour
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        initializeUI()
    }
    
    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
        initializeUI()
    }
    
    private func initializeUI() {
        self.translatesAutoresizingMaskIntoConstraints = false
        self.widthAnchor.constraint(equalTo: heightAnchor).isActive = true
        doCircleRadius()
        
        loadingCircle?.removeFromSuperlayer()
        loadingCircle = CAShapeLayer()
        loadingCircle!.lineWidth = 1
        loadingCircle!.fillColor = nil
        loadingCircle!.strokeColor = _lastRingColour.cgColor
        loadingCircle!.strokeStart = _lastSpinnerState.strokeStart
        loadingCircle!.strokeEnd = _lastSpinnerState.strokeEnd
        loadingCircle!.rasterizationScale = UIScreen.main.scale
        loadingCircle!.shouldRasterize = true
        layer.addSublayer(loadingCircle!)
        
        imageView = UIImageView()
        imageView.contentMode = .scaleAspectFit
        imageView.tintColor = .white
        addSubview(imageView)
        imageView.translatesAutoresizingMaskIntoConstraints = false
        imageView.centerXAnchor.constraint(equalTo: centerXAnchor).isActive = true
        imageView.widthAnchor.constraint(equalTo: widthAnchor, multiplier: 0.35).isActive = true
        imageView.heightAnchor.constraint(equalTo: imageView.widthAnchor).isActive = true
        imageViewCenterOffsetConstraint = imageView.centerYAnchor.constraint(equalTo: centerYAnchor)
        imageViewCenterOffsetConstraint.isActive = true
        
        oldImageView = UIImageView()
        oldImageView.contentMode = .scaleAspectFit
        oldImageView.tintColor = .white
        addSubview(oldImageView)
        oldImageView.translatesAutoresizingMaskIntoConstraints = false
        oldImageView.centerXAnchor.constraint(equalTo: centerXAnchor).isActive = true
        oldImageView.centerYAnchor.constraint(equalTo: centerYAnchor).isActive = true
        oldImageView.widthAnchor.constraint(equalTo: widthAnchor, multiplier: 0.35).isActive = true
        oldImageView.heightAnchor.constraint(equalTo: imageView.widthAnchor).isActive = true
        
        let rotation = CABasicAnimation(keyPath: "transform.rotation.z")
        rotation.fromValue = 0
        rotation.toValue = Double.pi * 2
        rotation.duration = 1
        rotation.repeatCount = .infinity
        rotation.isRemovedOnCompletion = false
        loadingCircle?.add(rotation, forKey: "loadingAnim")
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        layer.cornerRadius = min(bounds.size.width, bounds.size.height) / 2
        loadingCircle?.frame = bounds
        loadingCircle?.path = UIBezierPath(roundedRect: bounds, cornerRadius: layer.cornerRadius).cgPath
    }
    
    private func doCircleRadius() {
        layer.cornerRadius = min(bounds.size.width, bounds.size.height) / 2
    }
    
    func updateProgressState(with progressState: ProgressState, animated: Bool) {
        if let circle = loadingCircle {
            let previousStartValue = circle.strokeStart
            let previousEndValue = circle.strokeEnd
            circle.strokeStart = progressState.spinnerState.strokeStart
            circle.strokeEnd = progressState.spinnerState.strokeEnd
            if animated && progressState.spinnerState != _lastSpinnerState {
                let start = CABasicAnimation(keyPath: "strokeStart")
                start.fromValue = previousStartValue
                start.toValue = circle.strokeStart
                let end = CABasicAnimation(keyPath: "strokeEnd")
                end.fromValue = previousEndValue
                end.toValue = circle.strokeEnd
                let anim = CAAnimationGroup()
                anim.animations = [start, end]
                anim.isRemovedOnCompletion = true
                anim.duration = 0.3
                anim.timingFunction = CAMediaTimingFunction(name: kCAMediaTimingFunctionEaseInEaseOut)
                circle.add(anim, forKey: nil)
            }
            let previousStrokeColour = circle.strokeColor
            circle.strokeColor = progressState.ringColour.cgColor
            if animated && _lastRingColour != progressState.ringColour {
                let anim = CABasicAnimation(keyPath: "strokeColour")
                anim.fromValue = previousStrokeColour
                anim.toValue = circle.strokeColor
                anim.duration = 0.3
                circle.add(anim, forKey: nil)
            }
        }
        if animated {
            oldImageView.image = imageView.image
            oldImageView.alpha = 1
            imageView.alpha = 0
            imageViewCenterOffsetConstraint.constant = 50
            imageView.image = progressState.image
            layoutIfNeeded()
            imageViewCenterOffsetConstraint.constant = 0
            UIView.animate(withDuration: 0.3, delay: 0, options: .curveEaseInOut, animations: {
                self.layoutIfNeeded()
                self.imageView.alpha = 1
                self.oldImageView.alpha = 0
            }, completion: { _ in
                self.oldImageView.image = nil
            })
        } else {
            imageView.image = progressState.image
            oldImageView.image = nil
            oldImageView.alpha = 0
        }
        _lastSpinnerState = progressState.spinnerState
    }
    
}
