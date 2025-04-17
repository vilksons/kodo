### **What is this?**

This directory is the main and important directory of the Kodo Project. It contains one shell/Bash file and two Python files.

---

### **How does it work?**

By assigning various functions from everything that exists—both shell/Bash modules and Python modules. All of these functions are managed through the *workspace*, which is built on top of shell/Bash.

The *workspace* is the core file of Kodo and serves as the connector and central hub for all modules. It works by calling various modules using either the `source` command or `python3` execution.

**WPython** is a part of the Python module in the *workspace*, and stands for **Workspace Python**. WPython is a module that contains various exports from Python scripts—functions that either cannot be executed in shell/Bash or require more advanced capabilities not available in shell/Bash.

**VPython** is another part of the Python module in the *workspace*, and stands for **Vireon Python**. VPython is a component of Vireon A.I. that enhances the A.I.’s responses by adding more advanced features, such as syntax highlighting and other enhancements to improve the output.

---

### **What is `keywords.json`?**

`keywords.json` is a configuration file used by VPython to apply syntax highlighting dynamically. Instead of hardcoding keywords and their associated colors directly into the Python script, this file defines them externally in a simple and flexible JSON format.

Each key in the JSON file represents a color group (like `green`, `red`, `orange`, or `gray`), and its value is an array of keywords that should be displayed in that color. This structure allows easy customization and extension of syntax rules without modifying the codebase.

Here’s an example:

```json
{
  "green": ["if", "else", "true", "int", "float"],
  "red": ["false", "return", "break"],
  "orange": ["try", "catch", "finally"],
  "gray": ["git", "bash", "docker"]
}
```

VPython loads this file from a remote URL (e.g., a GitHub RAW link) and applies coloring based on it when rendering code blocks in AI-generated content. This makes the system more modular, maintainable, and user-extensible.