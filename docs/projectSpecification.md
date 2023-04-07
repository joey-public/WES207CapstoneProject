# WES 207: Capstone Project Specification

## Project Charter

### Project Overview

This project aims to use modern technology to track the positions of collard lizards in Arkansas. The goal is to set up several Software

### Project Approach

### Minimum Viable Product

For this project the minimum viable product is to demonstrate localization of a single transmitter in a lab setting. The MVP should have at least 2 USRP synced directly with a signal generator. To keep the scope of the project manageable we have broken it into 2 main tasks. 

First we will get two SDRs synchronized in the lab with a signal generator. We can then work on DSP algorithms that use phase information to obtain accurate time of arrival measurements. The MVP deliverable for this portion of the project is to have each USPR receive the radio signal from a sample transmitter and output an estimate a time of arrival for each SDR. 

In parallel to this we will develop a localization simulation. In a production system there would be a host computer that receives all the times of arrivals from each SDR and computes the position of the transmitter using time-difference of arrival calculations. For the minimum viable product we must simulate the production system. We should also write and document the code in a way that separates functions and classes that can be reused in future work from other code that might be used just for visualizations. The MVP requirement for this task is to have a simulation that takes in 2-4 time of arrival measurements and produces a postion estimation along with some GUI visualization for demonstration purposes.  

The deliverable for the full MVP is to take results froms tasks 1 and 2 and combine them into a single lab demonstration.  

If time permits we would also like to use GPSDOs to remotly synchronize the USRPs, but we are removing this as an MVP requirement because we are unsure on the timeline for when we will be able to get our hands on the GPSDO hardware. 

### Constraints Risks Feasibility


## Group Management


## Project Development


## Project Milestones and Schedule

