# WES-207 Capstone Project - Collared Lizard Lozalization for UCSD MAS-WES, cohort 10.

## Project Overview
This repository contains the code and documentation for our Capstone project as part of the UCSD Master of Advanced Studies in Wireless Embedded Systems (MAS-WES) program. The project aims to provide proof of concept study of localization of a lizard fitted with transmitter emitting complex sinusoidal signals between 100-200 MHz using 4 B200 USRPS. These B200 USRPs consists of internally mounted GPSDOs.
The project uses Boost::asio for Socket Programming. 

## Team Members
- [Vinit Saah](https://github.com/VinitSaah)
- [Joey Schnecker](https://github.com/joey-public)
- [Ryan Margraf](https://github.com/rmargraf142)
- [Hassan Ahmad](https://github.com/khahmad95)

## Folder Structure
- `docs/`: Contains project documentation, including any reports or presentations. Moreover, all the other documentation, how to related project code and scripts.
- `dsp/`: Contains python/matlab scripts of dsp developed for signal processing.
- `localization/`: Contains scripts and cross-correlation FFTW library used for localization
- `main/`: Contains main code for host and host controller app.
- `playground`: Hands on several concepts used for programming
- `prototype`: UHD (USRP Hardware Driver) API examples taken from UHD github which were used to unit test the functionalities.

## Installation and Setup
1. Clone the repository: `git clone https://github.com/joey-public/WES207CapstoneProject.git`
2. Navigate to the project directory: `cd main`
3. Follow README.md mentioned
4. Go to docs/setup for UHD installation guide, Eigen lib guide, and python_setup_guide

## Usage
Go to README.md in main folder

## Results
Go to docs/Project_Documentation folder and unzip to read Project Specification and other presentations. Presentations contain fortnightly updates leading to the MVP. It has all intermediate results documented. 

## Project Tracking:
https://capstone-wes207.atlassian.net/jira/software/projects/CLL/boards/1/roadmap

## Contributing
Currently it is a Capstone Project intended for internal use for UCSD MAS-WES. Target candidate of E4E.

## License
-- For internal use of UCSD MAS

## Acknowledgements
Professor Kastner at UCSD, Nathan from E4E and other MAS-WES faculty and staff members who enabled us to complete MVP in timely manner.

## Contact
For any questions or inquiries about this project, please contact [kastner@cs.ucsd.edu].





 
