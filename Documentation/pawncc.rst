Windows
======
Download the ZIP archive of pawncc and extract pawnc.dll, pawncc.exe, pawndisasmsm.exe to your original pawncc directory. If you use Pawno/Qawno, this will be in the pawno/ or qawno/ folder that's inside your server directory.

Linux - libpawnc.so installation
======

I. **x86_64 Package Support (For 64-bit Systems)**

   If you're using a 64-bit system and need support for x86_64 (32-bit) packages, make sure you install the required `libc` and `lib32` supporting packages. On Debian/Ubuntu-based systems, you can install these packages with the following command:

   .. code-block:: bash

        sudo apt-get install libc6:i386 lib32gcc1

   If you're using a different distribution, make sure to install the appropriate `x86_64` or `lib32` packages.

I. **Step-by-Step installation of libpawnc.so on Linux Env.**
   - **On Linux**:
     .. code-block:: bash
    
        sudo cp libpawnc.so /usr/local/lib/
        sudo ldconfig
        export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
        source ~/.bashrc

   - **On Termux**:
     .. code-block:: bash
    
        cp libpawnc.so /data/data/com.termux/files/usr/local/lib/
        export LD_LIBRARY_PATH=/data/data/com.termux/files/usr/local/lib:$LD_LIBRARY_PATH
        source ~/.bashrc

   - **Verify the Library Installation**

   .. code-block:: bash
        
        ldconfig -p | grep libpawnc

   If the library is listed, the installation was successful.

I. **If No Library is Detected, Move it to `/usr/local/lib32/`**

   If your library is not detected after running `ldconfig -p`, try moving it to the `/usr/local/lib32/` directory for 32-bit libraries. Use the following command:

   .. code-block:: bash
    
        sudo mv libpawnc.so /usr/local/lib32/

   Then run `ldconfig` again:

    .. code-block:: bash

        sudo ldconfig
   