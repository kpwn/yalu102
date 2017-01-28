IOKit's `Headers` directory from the macOS SDK is expected to be found here, named `IOKit`. Dependencies of IOKit (namely libkern) should also be made available through this directory.

The Xcode project will automatically create all required symlinks when compiling, but you're free to place your own directories here. They will be left alone.
