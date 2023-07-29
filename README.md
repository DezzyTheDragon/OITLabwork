# OITLabwork
A repository for some of my lab work while I was at The Oregon Institute of Technology.

<h2>Summary</h2>
After 4 years of hard work this is a small collection of the work I have done to demonstrate some of the things I have done and show my ability to use C and C++ and my comfort programming in linux


<h3>cst320_Compilers</h3>
This folder contains the work I did for my compiler class in 2022. Using C we wrote a basic compiler for the language pascalish, a simplified reduced version of pascal. The tests folders contain tests written to check to make sure that the compiler works.

<h3>cst407_ConcurrentProgramming</h3>
This folder contains the work I did for my concurrent programming class in 2023. A class focusing on how to properly take advantage of not only threads but also for MPI.
<ul>
    <li>
        Lab 2: Locks
        <ul style="list-style-type: none;">
            <li>A basic program that explores the basic usage of locking data sets. This lab looks at using no lock, coarse grain locking, reader-writer locking, and fine grain locking.</li>
        </ul>
    </li>
    <li>
        Lab 3: Threaded matrix multiply
        <ul style="list-style-type: none;">
            <li>This program focuses on taking a large expensive task and breaking it down in a way that will allow for efficent multithreading. This lab uses multiplying two large matrixes together as the expensive task.</li>
        </ul>
    </li>
    <li>
        Lab 4: MPI matrix multiply
        <ul style="list-style-type: none;">
            <li>The goal of this lab is to do the same as Lab 3 but instead of using threads it uses multiple machines using MPI. Again using the multiplication of two large matrixes together as the expensive task.</li>
        </ul>
    </li>
    <li>
        Lab 6: Lock implementation
        <ul style="list-style-type: none;">
            <li>This lab focuses on understanding how locks work and creating our own psudo implementation of locks, spinlocks, test and test and set, reader-writer with reads favored, and reader-writer with writes favored.</li>
        </ul>
    </li>
</ul>

<h3>cst415_Networking</h3>
This folder contains the work I did for my networking class in 2022. Understanding how networking works and how to implement communication using TCP and UDP protocols.
<ul>
    <li>
        Lab 2: Port manager
        <ul style="list-style-type: none;">
            <li>Multiple processes can not bind to the same port so this program gets requests and hands out a port to the requesting server process.</li>
        </ul>
    </li>
    <li>
        Lab 2: File transfer
        <ul style="list-style-type: none;">
            <li>A program that is very similar to other file transfer programs such as FileZilla, allowing connected clients download a specified file to the clients machine.</li>
        </ul>
    </li>
    <li>
        Lab 5: Threaded clients
        <ul style="list-style-type: none;">
            <li>Handles clients by giving each connected client its own thread so there can be lots of connected clients without effecting the main server thread.</li>
        </ul>
    </li>
</ul>
