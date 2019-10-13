# Biryani Serving

**Assumptions**

- only one table filled at once
- only one student assigned slot at once


# CODE LOGIC:

## ROBOT:

**robot function**
- runs in a thread of its own
- infinitely (untill main() terminates) prepairs food and fills `robot_a[id]   // available vessels with each robot` 
- once robot_a[id] is filled, `biriyani_ready()` is called, and then when the function returns, loop restarts

**biryani_ready**
- returns when robot_a[id] is empty (is 0);

## SERVING TABLE:

**table function**
- runs in a thread of its own, infinitely does these steps
- if table has slots and has food, it calls `ready_to_serve`  
- `ready_to_serve` returns when table is not ready to serve
- if table has all slots, and no food, it fills table by calling `fill`
- fill waits for available robot and fills the table assigned to be filled
- and thus Lord Voldemort was immortal

## STUDENT:

**student**
- calls wait for slot and gets a slot assigned
- after reaching and eating 

**wait_for_slot function**
- searches for an empty slot table with food
- gives the food in that table and assigns that slot to the student

**student_in_slot**
- student calls this function to release the slot he had occupied

# Thanks! :D

> Amey Kudari
> 2018101046