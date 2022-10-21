# Philosophers
42 School project with threads and mutex

Description :
The philosophers (1 minimum) are arround a table with a pasta plate in front of them.
They can eat, think or sleep.
They can only do one thing at a time.
There is one fork per philosopher on the table.
A philosopher needs 2 forks to start eating and he keeps them the entire duration of the meal.
When a philosopher finishes eating, he drops the forks back on the table and starts sleeping.
After his nap, he starts thinking and waits for 2 forks to be freed for him to be able to eat again.
The simulation ends if a philosopher dies.
Philosophers need to eat and if they don't eat in time they will die.

How to use :
- Open the terminal.
- Use the following commands :
git clone https://github.com/theoske/Philosophers.git philosopher
cd philosopher/philo/
make
- Now you can run the philo program with the arguments you want like this :
    ./philo   Number_of_philosophers   Time_to_die   Time_to_eat   Time_to_sleep   [Times_each_philosopher_must_eat]
- The last argument is optional.
