# Tiles 2

## Overview
This project is a sophisticated tile-based game engine built using Unreal Engine, designed to support dynamic real-time mesh merging, a versatile parsing system, and an advanced user interface for tile creation and manipulation. The system is modular, supporting various board shapes (Square, Triangle, Hex), and is highly customizable to allow for complex game mechanics and interactions.

## Key Features
#### 1. Real-Time Mesh Merging with Rounded Corners
  - Supports real-time mesh merging, dynamically combining tiles based on adjacency.
  - Includes advanced vertex management for rounded corners, ensuring smooth transitions between tiles.
  - Handles various board shapes (squares, triangles, hexagons) and adjusts vertex positions for consistency.

#### 2. Generalized Coordinate System
  - Implements a generalized coordinate system supporting multiple board shapes.
  - Each board shape has its own coordinate class for precise tile placement and interaction.
  - Designed to be extendable for adding new board shapes with minimal changes to the core system.

#### 3. Parsing System with Multiple Parse States
  - Features a robust parsing system with multiple parse states and syntax enforcement.
  - Uses variants to parse arbitrary variable types, handling inputs from simple numbers to complex data structures.
  - Highly adaptable parser that can interpret complex tile configurations from user inputs or scripts.

#### 4. Custom Logger
  - Integrates a custom logger with extensive features for debugging and development.
  - Supports different log levels, on-screen messages, and contextual information.
  - Provides immediate feedback through console and in-game UI, enhancing real-time monitoring.

#### 5. Creator Menu and Advanced Selection Modes
  - Powerful creator menu for selecting, copying, and manipulating tiles.
  - Includes multiple selection modes (Rectangle, Triangle, Circle) tailored to board shapes (Square, Triangle, Hex).
  - Allows rotation of the selection angle, dynamically adjusted based on board shape for natural selection.
  - Features copying tiles, copying tile signatures (including modules and properties), and multi-selection for complex operations.

#### 6. Inheritance-Based Board Shape Management
  - Manages each board shape through inheritance, allowing specialized populators for each shape.
  - Encapsulates tile population logic within each board shape class for maintainability and extensibility.
  - Future shapes or variations can be added easily by creating new subclasses, preserving modularity and scalability.

## Future Work

- **Level Creator:** The next phase of development will focus on completing the level creator, enabling users to design and customize levels with greater control and flexibility.
- **Gameplay Loop**: Work will begin on implementing the gameplay loop, integrating the tile mechanics into a cohesive game experience.
- **Additional Tile Modules**: New tile modules will be developed to expand the range of interactions and strategies available within the game.
- **Save and Reopen Levels**: Implement functionality to save and reopen user-created levels, allowing for persistent and revisitable game worlds.

## Conclusion
This tile-based game system provides a strong foundation for creating complex, dynamic game environments. Its advanced mesh merging, parsing capabilities, and user-friendly creator tools make it a powerful engine for developers and players alike. As development progresses, additional features will continue to expand its potential, paving the way for innovative gameplay experiences.
