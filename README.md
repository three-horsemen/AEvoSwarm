AEvoSwarm
===

Introduction
---

We have observed that most behaviors in real live beings, both plants, animals, and humans alike, are developed over a period of time. These are heavily influenced with culture and inherited characteristic traits, which are in turn influenced by the same of their predecessors. Here, we extrapolate these observations to the idea that most modern rational schools of thought, such as military strategy, colonization, and even basic survival instincts, are honed over a multitude of generations, passing their knowledge and base character from generation to generation. With inherited genetic error, the newer generations imbibe the ability to form smarter and more informed decisions about the world around them. And by Darwin’s theory of “survival of the fittest”, we expect to see that the agents that fail to learn over the progress of time are quickly eliminated. Here, the core purpose of an agent is to persist its existence. So, the agents that fail to perform life actions to keep up to this purpose will eventually fade from existence.

Objective
---

The objective of this project is to establish a framework through which, given the model of any system consisting of an environment and the underlying mechanisms governing the functionality of any individual agent, a simulation can conveniently and efficiently be executed so as to achieve a large real time factor which is crucial to the success of the simulation.
The second major objective is to study the types of emergent behaviour among the agents that comes about as a result of the evolutionary process and to design a system that encourages complex and advanced emergent behaviour without any explicit specification.

Framework
---

In our work, we establish a framework through which a system consisting of a swarm of autonomous agents along with their environment are modelled and simulated. All such agents in this system undergo the same processes sharing the same underlying mechanism of functioning. This property of a shared underlying mechanism inspires the employment of a very scalable SIMD based approach to drive the underlying processes in a manner that can scale efficiently with the number of agents in the system.

Scope - Ant World
---

For the purposes of this project, we have decided to limit the scope of the system to that of colonies of ants, and thereby follow the common English phrase, “a swarm of ants”. Here, the agent in consideration would be the ant. Each ant will be capable of performing exactly one of a finite number of actions vital to its survival, in a turn based manner including motion, consumption of food, attacking, and reproducing to create progeny with slight mutations in it’s own behaviour, where it’s behaviour is governed by a neural network. An ant requires energy to live in the world, and the act of living in itself costs energy. With a finite amount of energy in the world, the ant must discover ways to survive in the world around it. As generations progress, we expect to observe the ants to learn that forming colonies is the best way to collectively survive for more generations. This hypothesis has a biological significance as a result of the co-evolution of colonies and swarms of a given species including complex social interactions observed in nature.
We hope to observe new and exciting behaviors that emerge from the swarms over time, given a set of life functions and base requirements for survival.

Applications of parallelism
---

The usage of parallel processing has major implications with respect to the success of this project as a result of the
SIMD-based framework described above. Thus the processed of each ant can be simulated on a separate GPU thread.
Moreover, since the receptive field of an ant will be restricted to only a few tiles around it, we can make the
assumption that a given ant can function independently of the ants outside its receptive field. This will prevent the
need for excessive synchronization among the threads.

![](https://media.githubusercontent.com/media/three-horsemen/AEvoSwarm/master/screenshots/mpi%20-n%204%20with%20task%20manager.png)

From generation of food on the map to handling the interactions of ants on the map, can also be parallelized to a great
extent. Further, dividing the map into logical sectors, each of which can individually host multiple tiles upon which
the ants can coexist provides scope for increasing the parallelization further.

Setup
---

This project utilizes a variety of computational methods including multiprocessing in MPI and GPU programming in OpenCL,
and performs a variety of functionalities such as GTK window management, graphics in OpenCV and also utilizes the boost
libraries to perform various miscellaneous functions such as multithreading, OS interaction and logging. As a result,
there are a variety of dependencies that need to be installed. Installation of the required dependencies on Ubuntu have
been documented below:

Install dependencies:

    sudo apt install libboost-all-dev mpi ocl-icd-opencl-dev

You will also need to install opencv, but ensure you have these dependencies before you do:

    sudo apt install libgtk2.0-dev pkg-config

There are a variety of online guides on how to do this. For example, step 3
of [this](https://www.pyimagesearch.com/2015/07/20/install-opencv-3-0-and-python-3-4-on-ubuntu/) guide should suffice
for our purposes.

Once you have these installed you can proceed to compilation.

This project is structured like you typical CMake project. Most modern IDEs should thus be able to natively support
compiling and generation of build artifacts that you can execute.

Alternatively, running it from command line:

    mkdir build
    cd build
    cmake ..
    make

Usage
---

Now that you have your executables generated, we recommend starting with one of the main executable files:

    cd ..
    ./build/unit_test_ant_world
    # Press escape to exit gracefully

    # The below executable will parallelize the evolution in two processes
    ./build/unit_test_ant_world_mpi -n 2 unit_test_ant_world_mpi

There are also a variety of other unit tests that would be generated that help you troubleshoot the various
functionality such as OpenCL if the main simulation is not executing. You are likely missing a dependency installation
or have incompatible system specifications.
