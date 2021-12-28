# MSP_Mobile_Model_Viewer

# Test Setup
* Converter PC:
    * Intel i7-7700HQ (Mobile, 8-Core, 2.8 GHz)
* Phone:
    * Honor 9 (Kirin 960, 8-Core, 4x 2.4 GHz, 4x 1.8 GHz)

## Measured Data
All Measured data used the tank model.
Render Time => Time to render the object 10 times.

### Converter
* Vertices: **1625868**
* Conversion Time: **742 ms**

### Obj Loader
Used loader: https://github.com/Bly7/OBJ-Loader
Loader was modified to be used on android.
* Load Time: **135.253 s**
* Render Time: **102 ms**

### Bin Loader
Optimizations: Index Buffer
* Load Time: **2.105 s**
* Render Time: **2.3 ms**