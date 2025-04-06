KODO Toolchain
==============

Wiki Guide
----------

Essential references to understand and use Kodo:

- `Compiler Options <https://github.com/vilksons/kodo/wiki/Compiler-Option>`_
- `Required Packages <https://github.com/vilksons/kodo/wiki/Required-Packages>`_
- `PawnCC Installation <https://github.com/vilksons/kodo/wiki/PawnCC-Installation>`_
- `CODE OF CONDUCT <https://github.com/vilksons/kodo/wiki/CODE-OF-CONDUCT>`_

What is Kodo?
-------------

- `Kodo <https://github.com/vilksons/kodo/tree/kodo>`_  
  Kodo is an SA:MP-specific toolchain designed for debugging, compiling, and package management.

What is cKodo?
--------------

- `cKodo <https://github.com/vilksons/kodo/tree/ckodo>`_  
  cKodo is a Kodo Toolchain built on the C language without any connection to Shell/Bash.

Keeping Up-to-Date
------------------

Always update using:

.. code-block:: bash

   sync

Prerequisites
=============

Installation
------------

**Supported Platforms:**

- Linux, Darwin, Windows, macOS, and Android/iPhone via:

  - `Docker <https://www.docker.com/>`_
  - `WSL/WSL2 <https://learn.microsoft.com/en-us/windows/wsl/install>`_
  - `MSYS2 <https://www.msys2.org/>`_
  - `mingw-w64 <https://www.mingw-w64.org/>`_
  - `Cygwin <https://www.cygwin.com/>`_
  - `Babun <https://github.com/babun/babun>`_
  - `Termux <https://termux.dev/en/>`_
  - `UserLAnd <https://github.com/CypherpunkArmory/UserLAnd>`_
  - `Linux Deploy <https://github.com/meefik/linuxdeploy>`_
  - `iSH <https://github.com/ish-app/ish>`_
  - `a-Shell <https://github.com/holzschu/a-shell>`_

Install `kodo/workspace` Script
-------------------------------

**Via GNU/Wget:**

.. code-block:: bash

   wget -q --show-progress -O workspace https://raw.githubusercontent.com/vilksons/kodo/refs/heads/kodo/Scripts/workspace

**Via cURL:**

.. code-block:: bash

   curl -L -o workspace https://raw.githubusercontent.com/vilksons/kodo/refs/heads/kodo/Scripts/workspace

Install Required Packages
-------------------------

Full list available at:  
`Required Packages <https://github.com/vilksons/kodo/wiki/Required-Packages>`_

Installation Steps
------------------

.. code-block:: bash

   1. Open the Terminal.
   2. chmod +x workspace
   3. bash ./workspace

Docker Usage
============

MacOS
-----

.. code-block:: bash

   docker run -it ubuntu
   docker run -v ~/Downloads:/mnt/downloads -it ubuntu

   apt update
   apt upgrade # optional
   dpkg --add-architecture i386
   apt update
   apt install libc6:i386 libstdc++6:i386
   apt install python3 wget curl grep awk sed coreutils yq jq aria2 unzip tar 

   cd /path/to/kodo/
   chmod +x workspace
   ./workspace

Linux/WSL2
----------

.. code-block:: bash

   sudo usermod -aG docker $USER
   newgrp docker
   sudo systemctl start docker
   sudo systemctl enable docker

   docker run -it ubuntu
   docker run -v ~/Downloads:/mnt/downloads -it ubuntu

   apt update
   apt upgrade
   dpkg --add-architecture i386
   apt update
   apt install libc6:i386 libstdc++6:i386
   apt install python3 wget curl grep awk sed coreutils yq jq aria2 unzip tar

   cd /path/to/kodo/
   chmod +x workspace
   ./workspace

Additional Docker Commands
--------------------------

.. code-block:: bash

   docker ps -a
   docker start <container-name>
   docker exec -it <container-name> /bin/bash
   docker stop <container-name>
   docker rm -f <container-name>
   docker rm -f $(docker ps -a -q)

Usage
=====

File Renaming
-------------

Rename ``yourmode.pwn`` to something like ``yourmode.io.pwn`` for clarity.

Compilation
-----------

.. code-block:: bash

   compile .                      # Default compilation
   compile yourmode.pwn           # Specific file compilation
   compile yourmode.pwn opt1 opt2 opt3  # With options

Starting & Debugging
--------------------

.. code-block:: bash

   running yourmode.pwn
   debug yourmode.pwn

Configuration: ``kodo.json``
----------------------------

+---------------------+-------------------------------------------------------------------------------------------+
| Key                 | Description                                                                               |
+=====================+===========================================================================================+
| ``amx_flags``       | Compiler option flags. See `Compiler Options <https://github.com/vilksons/kodo/wiki/Compiler-Option>`_ |
| ``include_paths``   | Directory path for additional includes used in compilation.                              |
| ``exclude_paths``   | Paths to exclude from compilation.                                                        |
| ``samp_log``        | SA-MP server log file path.                                                               |
| ``samp_config``     | SA-MP configuration file (for TKodo).                                                    |
| ``samp_init``       | SA-MP main server file (for TKodo).                                                      |
| ``samp_executable`` | Path to the SA-MP server executable.                                                     |
| ``include_dir``     | Directory for includes managed by the package manager (TKodo).                           |
| ``plugins_dir``     | Directory where ``.dll`` and ``.so`` plugin files are stored (TKodo).                    |
| ``bot_token``       | API key from `Groq Console <https://console.groq.com/keys>`_                             |
| ``bot_model``       | AI model from `Groq Console <https://console.groq.com/>`_                                |
| ``bot_profile``     | Additional chatbot configuration info.                                                   |
| ``remcache_auto``   | Automatically remove `.cache`, table history, etc. (``true``/``false``)                  |
| ``repository``      | Repository source for sync. (``stable`` or ``testing``)                                  |
+---------------------+-------------------------------------------------------------------------------------------+

**Example for ``include_paths``:**

If your script uses ``includes/``, set:

.. code-block:: json

   "include_paths": "includes"

**Example Directory Structure:**

.. code-block:: text

   samp-server/
   ├── gamemodes/
   │   └── mygamemode.pwn
   ├── includes/
   │   └── customFile.pwn
   └── server.cfg

Package Manager - TKodo
=======================

Modes
-----

- Kodo Mode (default)
- TKodo Mode (with package management)

Switch Modes
------------

.. code-block:: bash

   tkodo    # Enter TKodo Mode
   exit     # Exit TKodo Mode

Manage Packages
---------------

.. code-block:: bash

   install github/samp-incognito/samp-streamer-plugin
   remove streamer
   autoinstall

TKodo Configuration
-------------------

**tkodo.toml**

.. code-block:: toml

   [package]
   urls = [
     "github/user/repository",
     "gitlab/user/repository",
     "sourceforge/user/repository"
   ]

**tkodo.json**

.. code-block:: json

   {
     "package": [
       "github/user/repository",
       "gitlab/user/repository",
       "sourceforge/user/repository"
     ]
   }

Security
========

TKodo uses:

- Double SHA256 hashing (e.g., for bot tokens)
- Encrypted cache
- aria2 for fast, secure downloads
- Smart validation for `.so`, `.dll`, `.inc` files
- Cleanup for unknown or unnecessary install files

Final Notes
===========

Keep updated using:

.. code-block:: bash

   sync
   # or
   syncc

Resources:

- `Kodo Wiki <https://github.com/vilksons/kodo/wiki>`_
- `GitHub Discussions <https://github.com/vilksons/kodo/discussions>`_
- `Report Bugs <https://github.com/vilksons/kodo/issues>`_
