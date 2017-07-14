//
//  ProgressView.swift
//  yalu102
//
//  Created by AppleBetas (ported by mullak99)
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

import UIKit

class ProgressView: UIView {
    private var titleLabel: UILabel!, progressCircleView: ProgressCircleView!
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        initializeUI()
    }
    
    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
        initializeUI()
    }
    
    private func initializeUI() {
        let titleContainerVibrancyView = UIVisualEffectView(effect: UIVibrancyEffect(blurEffect: UIBlurEffect(style: .dark)))
        titleContainerVibrancyView.translatesAutoresizingMaskIntoConstraints = false
        addSubview(titleContainerVibrancyView)
        titleContainerVibrancyView.topAnchor.constraint(equalTo: topAnchor).isActive = true
        titleContainerVibrancyView.leftAnchor.constraint(equalTo: leftAnchor).isActive = true
        titleContainerVibrancyView.rightAnchor.constraint(equalTo: rightAnchor).isActive = true
        
        titleLabel = UILabel()
        titleLabel.font = .systemFont(ofSize: 28, weight: UIFontWeightLight)
        titleLabel.textColor = .white
        titleLabel.textAlignment = .center
        titleLabel.translatesAutoresizingMaskIntoConstraints = false
        titleContainerVibrancyView.contentView.addSubview(titleLabel)
        titleLabel.topAnchor.constraint(equalTo: titleContainerVibrancyView.contentView.topAnchor).isActive = true
        titleLabel.leftAnchor.constraint(equalTo: titleContainerVibrancyView.contentView.leftAnchor).isActive = true
        titleLabel.bottomAnchor.constraint(equalTo: titleContainerVibrancyView.contentView.bottomAnchor).isActive = true
        titleLabel.rightAnchor.constraint(equalTo: titleContainerVibrancyView.contentView.rightAnchor).isActive = true
        
        let progressCircleContainerView = UIView()
        progressCircleContainerView.translatesAutoresizingMaskIntoConstraints = false
        addSubview(progressCircleContainerView)
        progressCircleContainerView.topAnchor.constraint(equalTo: titleContainerVibrancyView.bottomAnchor, constant: 28).isActive = true
        progressCircleContainerView.bottomAnchor.constraint(equalTo: bottomAnchor).isActive = true
        progressCircleContainerView.leftAnchor.constraint(greaterThanOrEqualTo: leftAnchor).isActive = true
        progressCircleContainerView.rightAnchor.constraint(lessThanOrEqualTo: rightAnchor).isActive = true
        progressCircleContainerView.centerXAnchor.constraint(equalTo: centerXAnchor).isActive = true
        let widthConstraint = progressCircleContainerView.widthAnchor.constraint(equalToConstant: 140)
        widthConstraint.priority = 999
        widthConstraint.isActive = true
        
        progressCircleView = ProgressCircleView()
        progressCircleContainerView.addSubview(progressCircleView)
        progressCircleView.leftAnchor.constraint(equalTo: progressCircleContainerView.leftAnchor).isActive = true
        progressCircleView.rightAnchor.constraint(equalTo: progressCircleContainerView.rightAnchor).isActive = true
        progressCircleView.topAnchor.constraint(equalTo: progressCircleContainerView.topAnchor).isActive = true
        progressCircleView.bottomAnchor.constraint(equalTo: progressCircleContainerView.bottomAnchor).isActive = true
    }
    
    // lazy for now
    private var _progressState = ProgressState(text: "", image: nil, spinnerState: .none, overrideRingColour: nil)
    var progressState: ProgressState {
        get { return _progressState }
        set(progressState) { updateProgressState(with: progressState, animated: true) }
    }
    
    func updateProgressState(with progressState: ProgressState, animated: Bool) {
        _progressState = progressState
        
        if animated {
            let transition = CATransition()
            transition.type = kCATransitionFade
            transition.duration = 0.3
            transition.timingFunction = CAMediaTimingFunction(name: kCAMediaTimingFunctionEaseInEaseOut)
            transition.fillMode = kCAFillModeBoth
            transition.isRemovedOnCompletion = true
            titleLabel.layer.add(transition, forKey: "textAnimation")
        }
        titleLabel.text = progressState.text
        
        progressCircleView.updateProgressState(with: progressState, animated: animated)
    }
    
}
