<a id="readme-top"></a>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/sanjay-vcloud/keylogger">
    <img src="img/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Best-Linux-KeyLogger</h3>

  <p align="center">
    A lightweight and stealthy keylogger for Linux systems.
    <br />
    <a href="https://github.com/sanjay-vcloud/keylogger"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/sanjay-vcloud/keylogger">View Demo</a>
    &middot;
    <a href="https://github.com/sanjay-vcloud/keylogger/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/sanjay-vcloud/keylogger/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
        <li><a href="#features">Features</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

## About The Project

![Product Name Screen Shot][product-screenshot]

There are many keylogger available for linux systems, but most of them are not stealthy or efficient. This project aims to provide a lightweight, efficient, and stealthy keylogger for Linux systems that can capture keystrokes and send them to a remote server for analysis. It is designed to run as a daemon, allowing it to operate in the background without user intervention.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Built With
- [![C][c-url]][c-url]
- [![Libcurl][libcurl-url]][libcurl-url]
- [![Make][make-url]][make-url]
- [![GCC][gcc-url]][gcc-url]
<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Features

- Captures key press events in real-time.
- custom key-mapping support.
- Logs key events to a file at local system.
- Sends captured logs to a remote server every 5 minutes.
- Supports log rotation to manage log file size.
- Daemon mode for continuous logging and remote exfilteration.
- even it monitors continous in loop it not consume much memory and cpu usage.
- it works in blocking mode, so it will not miss any key press events.
- It is written in C language, making it lightweight and efficient.
- It is designed to be run as a daemon, allowing it to run in the background without user intervention.
<p align="right">(<a href="#readme-top">back to top</a>)</p>


## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

- GCC compiler
- Make
- Libcurl(for sending logs to a remote server)
- Root privileges (to run the keylogger)
- Linux system (tested on Ubuntu, Debian, and CentOS)

### Installation
1. Clone the repository:
    ```bash
    git clone https://git.selfmade.ninja/sanjay057/keytrace.git
    ```
2. Navigate to the project directory:
    ```bash
    cd keytrace
    ```
3. Install the required dependencies:
    ```bash
    make install
    ```
4. Compile the project:
    ```bash
    make
    ```
5. Configure the keylogger by editing the `config.h` file to set the remote server URL and other parameters.
6. Start the keylogger:
    ```bash
    sudo ./trace
    ```
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Usage

To use the keylogger, follow this steps:
1. Ensure you have the necessary permissions to run the keylogger (root privileges).
2. Start the keylogger by running the following command:
    ```bash
    sudo ./trace
    ```
3. The keylogger will start capturing keystrokes and logging them to the specified file.
4. To stop the keylogger, you can use the `kill` command to terminate the process:
    ```bash
    sudo kill $(cat /var/run/keylogger.pid)
    ```
5. The captured logs will be stored in the `/var/tmp/.syslog/.ktrace.log` directory by default. You can change the log file path in the `config.h` file.
6. To send the captured logs to a remote server, ensure that the server URL is correctly set in the `config.h` file. The keylogger will automatically send the logs every 5 minutes, then you can check the logs on the remote server.
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Top contributors:

<a href="https://github.com/sanjay-vcloud/keylogger/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=sanjay-vcloud/keylogger" alt="contrib.rocks image" />
</a>

<p align="right">(<a href="#readme-top">back to top</a>)</p>



## Licence

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contact
Sanjay Telegram - [@sanjayv057](https://t.me/sanjayv057)




<!-- MARKDOWN LINKS & IMAGES -->

[product-screenshot]: img/keylogger.png
[c-url]: https://img.shields.io/badge/C-blue?style=flat-square&labelColor=24292f
[libcurl-url]: https://img.shields.io/badge/Libcurl-green?style=flat-square&labelColor=24292f
[make-url]: https://img.shields.io/badge/Make-orange?style=flat-square&labelColor=24292f
[gcc-url]: https://img.shields.io/badge/GCC-red?style=flat-square&labelColor=24292f

