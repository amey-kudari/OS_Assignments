# OBER CABS

**Assumptions**

- cab in waiting state can wither become premium or non premium cab.
- if 2 riders come in a same cab, driver choose optimal path to make sure both reach in their riding time.


# CODE LOGIC:

## RIDER:

- When created, rider gets put into a queue (or premium queue based on cab type).
- rider thread then waits for rider's wait time and then checks if he has been assigned a cab
- if not assigned, rider gets removed and gets a timed out message
- if assigned, nothing happens, thread terminates
- this is checked by function: `fnrem(id)` and `pfnrem(id)` ( find and remove, and premium fnrem)


## DRIVER:

> I took a reverse approach by making the cab drivers search for riders, hence driver thread does most of the computations

- runs in a thread of its own, infinely 
- has 3 states: 0 (empty), 1(single rider), 2(2 riders/1 premium rider)
- infinitely, if state is 0, he checks pqueue and then queue for any available riders, if available his state changes to 1 or 2 based on the rider
- if state changed to 2, (premium rider), `same` is changed to 1 to make sure only one payment process is initiated
- if changed to 1, rider continues to drive, and checks for new rider simultaniously. if first ride's ride is completed, it switches to state 0
- if rider in state 1 finds another rider, its state changes to state 2, and same=0 (non premium). Here, r2 becomes the fastest finishing time among the two.

### PAYMENT SERVER:

- First, driver waits for a free server, then calls a thread finishes payment
- When driver calls thread, thread waits for free server and immedietly the driver releases the server
- Driver leaves and thread continues to complete payment in 2 seconds


# Thanks! :D

> Amey Kudari
> 2018101046