//
//  DrawerToggleView.swift
//  yalu102
//
//  Created by AppleBetas (ported by mullak99)
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

import UIKit

class DrawerToggleView: UIView {
    private var chevronImageView: UIImageView!
    weak var delegate: DrawerToggleViewDelegate?
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        initializeUI()
    }
    
    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
        initializeUI()
    }
    
    private func initializeUI() {
        layer.cornerRadius = 4
        
        let tap = UITapGestureRecognizer(target: self, action: #selector(DrawerToggleView.tapped))
        addGestureRecognizer(tap)
        
        chevronImageView = UIImageView(image: UIImage(named: "drawerToggle"))
        chevronImageView.translatesAutoresizingMaskIntoConstraints = false
        chevronImageView.contentMode = .scaleAspectFit
        chevronImageView.tintColor = .white
        addSubview(chevronImageView)
        chevronImageView.centerXAnchor.constraint(equalTo: centerXAnchor).isActive = true
        chevronImageView.centerYAnchor.constraint(equalTo: centerYAnchor, constant: -2).isActive = true
        chevronImageView.widthAnchor.constraint(equalToConstant: 16).isActive = true
        chevronImageView.heightAnchor.constraint(equalTo: chevronImageView.widthAnchor).isActive = true
    }
    
    func tapped() {
        delegate?.drawerToggleViewTapped(self)
    }
    
    var isOpen = false {
        didSet {
            chevronImageView.transform = isOpen ? CGAffineTransform.identity.rotated(by: .pi) : .identity
        }
    }
    
}

protocol DrawerToggleViewDelegate: class {
    func drawerToggleViewTapped(_ view: DrawerToggleView)
}
