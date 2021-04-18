[![Build Status](https://travis-ci.com/taylor-santos/portal-chess.svg?branch=trunk)](https://travis-ci.com/taylor-santos/portal-chess)
[![Coverage Status](https://coveralls.io/repos/github/taylor-santos/portal-chess/badge.svg?branch=trunk&kill_cache=1)](https://coveralls.io/github/taylor-santos/portal-chess?branch=trunk)

<h3 align="center">Portal Chess</h3>

<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>

<!-- GETTING STARTED -->

## Getting Started

### Prerequisites

If building on Windows, install [vcpkg](https://github.com/microsoft/vcpkg).

* CMake >= 3.16
* glfw3 >= 3.3

  Linux:
  ```sh
  apt install libglfw3-dev
  ```
  Windows:
  ```sh
  vcpkg.exe install glfw3:x64-windows-static
  ```

### Installation

1. Clone the repo

   Make sure to include the `--recurse-submodules` flag to clone the required submodules.
    ```sh
    git clone --recurse-submodules https://github.com/taylor-santos/portal-chess
    ```

1. Run CMake
    ```sh
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    ```
1. Build
    ```sh
    cmake --build .
    ```

<!-- CONTRIBUTING -->

## Contributing

1. Fork the Project
1. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
1. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
1. Push to the Branch (`git push origin feature/AmazingFeature`)
1. Open a Pull Request

<!-- LICENSE -->

## License

Distributed under the MIT License. See `LICENSE` for more information.

<!-- CONTACT -->

## Contact

Taylor Santos - taylor.p.santos@gmail.com

Project Link: [https://github.com/taylor-santos/portal-chess](https://github.com/taylor-santos/portal-chess)

<!-- ACKNOWLEDGEMENTS -->

## Acknowledgements

* [Dear ImGui](https://github.com/ocornut/imgui)
