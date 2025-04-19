# Task Configuration Guide

## Overview

This configuration file defines tasks for Visual Studio Code (VS Code). It allows you to set up and customize tasks, such as running shell scripts, building projects, and more. The configuration format is a JSON object.

### JSON Structure

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Run Kodo/Workspace",
      "type": "shell",
      "command": "${workspaceRoot}/workspace",
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "presentation": {
        "echo": true,
        "reveal": "always",
        "focus": false,
        "panel": "shared"
      },
      "problemMatcher": []
    }
  ]
}
```

---

### Field Descriptions

#### `version` (String)
- **Description**: Specifies the version of the task configuration.
- **How to Fill**: Always use `"2.0.0"` as the value for this field.
- **Example**: `"version": "2.0.0"`

#### `tasks` (Array)
- **Description**: Contains an array of task objects. Each task defines a specific action or command to run in VS Code.
- **How to Fill**: Each task should be defined as an object within the array. The example shows one task that runs a shell script.
- **Example**:
  ```json
  "tasks": [
    {
      "label": "Run Shell Script",
      "type": "shell",
      ...
    }
  ]
  ```

---

### Task Object Fields

#### `label` (String)
- **Description**: The name or label of the task that appears in the VS Code UI.
- **How to Fill**: Provide a short and descriptive name for the task.
- **Example**: `"label": "Run Shell Script"`

#### `type` (String)
- **Description**: The type of the task, which defines how VS Code runs the task.
- **How to Fill**: Common types include `"shell"`, `"process"`, and others. In this case, use `"shell"` for running shell scripts.
- **Example**: `"type": "shell"`

#### `command` (String)
- **Description**: The shell command or script to run.
- **How to Fill**: Provide the command or script path relative to the workspace root. For example, to run a script located in the workspace, use `"${workspaceRoot}/path/to/script"`.
- **What Not to Fill**: Do **not** leave this field empty; a valid command is required to execute the task.
- **Example**: `"command": "${workspaceRoot}/workspace"`

#### `group` (Object)
- **Description**: Defines the task’s group, which specifies its type and if it is the default task for the group.
- **How to Fill**: Use the `"kind"` field to specify the task group, and `"isDefault"` to indicate whether this task is the default for that group.
- **Example**:
  ```json
  "group": {
    "kind": "build",
    "isDefault": true
  }
  ```

#### `presentation` (Object)
- **Description**: Defines how the task’s output is displayed in the terminal.
- **How to Fill**: Customize the presentation by setting the following values:
  - `echo`: If `true`, the command is echoed in the terminal.
  - `reveal`: Controls when the terminal panel is revealed. Use `"always"`, `"silent"`, or `"never"`.
  - `focus`: If `false`, the terminal does not take focus.
  - `panel`: Determines whether to use a new or shared terminal panel.
- **Example**:
  ```json
  "presentation": {
    "echo": true,
    "reveal": "always",
    "focus": false,
    "panel": "shared"
  }
  ```

#### `problemMatcher` (Array)
- **Description**: Defines how problems in the output should be matched and presented in the Problems panel.
- **How to Fill**: Leave this empty if no problem matching is required for the task. For most simple tasks, an empty array `[]` is sufficient.
- **Example**: `"problemMatcher": []`

---

### Guidelines

- **Required Fields**:
  - `"version"`
  - `"tasks"`
  - `"label"`
  - `"type"`
  - `"command"`

- **Fields That Can Be Customized**:
  - `"group"`
  - `"presentation"`
  - `"problemMatcher"`

- **Avoid Filling**:
  - Do **not** leave critical fields empty like `"command"` or `"label"`.
  - Ensure that `"problemMatcher"` is left empty (`[]`) if you're not using any.

---

### Example Task Configuration

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Run Build Script",
      "type": "shell",
      "command": "${workspaceRoot}/scripts/build.sh",
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "presentation": {
        "echo": true,
        "reveal": "always",
        "focus": false,
        "panel": "shared"
      },
      "problemMatcher": []
    }
  ]
}
```

---

### Conclusion

This file allows you to set up tasks that can be run in Visual Studio Code's terminal. You can modify these tasks based on your needs by filling in the appropriate values and following the guidelines. Make sure you don't leave essential fields blank to avoid errors during task execution.

