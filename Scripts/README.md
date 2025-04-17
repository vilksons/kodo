### **What is this?**

This directory is the main and important directory of the Kodo Project. It contains one shell/Bash file and two Python files.

---

### **How does it work?**

By assigning various functions from everything that exists—both shell/Bash modules and Python modules. All of these functions are managed through the *workspace*, which is built on top of shell/Bash.

The *workspace* is the core file of Kodo and serves as the connector and central hub for all modules. It works by calling various modules using either the `source` command or `python3` execution.

**WPython** is a part of the Python module in the *workspace*, and stands for **Workspace Python**. WPython is a module that contains various exports from Python scripts—functions that either cannot be executed in shell/Bash or require more advanced capabilities not available in shell/Bash.

**VPython** is another part of the Python module in the *workspace*, and stands for **Vireon Python**. VPython is a component of Vireon A.I. that enhances the A.I.’s responses by adding more advanced features, such as syntax highlighting and other enhancements to improve the output.