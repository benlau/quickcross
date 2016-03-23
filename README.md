QML Cross Platform Utility Library
==================================

QuickCross is a collection of helper utility for writing cross platform QML application.

Components:

 1. Device - Device information provider
 2. StandardPaths - Wrapper of QStandardPaths for QML
 3. Rect - Wrapper of QRectF for QML
 4. Line - Wrapper of QLineF for QML

[Class Reference](http://benlau.github.io/quickcross/)

Testing Components:

 1. ImageReader - Wrapper of QImageReader
 2. JsonReader - Read a json file and convert to Javascript object
 3. ImageProvider - A custom image provider that can preload all the images.
 4. EasingCurve - Wrapper of QEasingCurve

Those componetns are already available but no document yet. And their API is not frozen for changes.

Installation
============

    qpm install com.github.benlau.quickcross

Related Projects
----------------
 1. [benlau/quickpromise](https://github.com/benlau/quickpromise) - Promise library for QML
 2. [benlau/quickflux](https://github.com/benlau/quickflux) - Message Dispatcher / Queue for Qt/QML
 3. [benlau/qsyncable](https://github.com/benlau/qsyncable) - Synchronize data between models
 4. [benlau/testable](https://github.com/benlau/testable) - QML Unit Test Utilities
 5. [benlau/qtci](https://github.com/benlau/qtci) -  A set of scripts to install Qt in Linux command line environment (e.g travis)
