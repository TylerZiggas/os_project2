import math

# load the doc, this one has 16 (2^4) entries, so no need to pad
input_doc = [1] * 16   # all values in this list are '1' for simplicity

# this is just pretend, but a global variable here will act the same as shared mem for this purpose
shared_mem_int_list = input_doc.copy()
# show the list, to verify we set it up correctly
print("using this list to test:")
print(shared_mem_int_list)

length_of_input_list = len(shared_mem_int_list)
depth_of_input_list = math.log(length_of_input_list, 2)
print("The total number of ints to sum is", length_of_input_list)
print("The total depth of the tree is", depth_of_input_list)

# pretend shared memory
# keeps track of the gap between integers at a given depth
# we start at 1 because at the max depth of the tree, we add index 0 and 1, 2 and 3, etc.
# at the next depth, we add index 0 and 2, 4 and 6, etc.
# at the next depth, we add index 0 and 4, 8 and 12, etc.
depth_increment = 1


def exec_bin_adder(index, depth):
    """Simulates a child process doing work after having been given xx (index) and yy (depth).
    Instead of reading from shared memory, it reads from a global variable.
    That is just mechanics to change in the c version."""
    a_index = index
    # TODO: calculate this from 'depth' (yy in the problem description),
    # until you do that, the depth param above is not used, which is likely incorrect
    gap = depth_increment
    b_index = a_index + gap
    a_value = shared_mem_int_list[a_index]
    b_value = shared_mem_int_list[b_index]
    print("working on int pair at indexes", (a_index, b_index),
          "with values", (a_value, b_value))

    updated_value = a_value + b_value
    shared_mem_int_list[index] = updated_value

    print("updated shared memory at position",
          index, "to value", updated_value)


def update_increment():
    """Updates the 'shared mem' depth_increment above. We always multiply the previous value by 2, except
    for the very first time. Probably a smarter algorithm than this, but it works."""

    global depth_increment
    if depth_increment == 1:
        depth_increment += 1
    else:
        depth_increment *= 2

    print("now incrementing indexes by", depth_increment)


def master():
    index = 0
    depth = depth_of_input_list

    while depth > 0:
        i = index
        while i < len(shared_mem_int_list):
            # TODO: WAIT() here, to make sure you can call this.  check some max_process var here
            exec_bin_adder(i, depth)
            # for the next iteratio of the while loop, jump to the next i after b, and after another depth_increment
            i = i + depth_increment + depth_increment

        # TODO: NEED TO WAIT() HERE TO LET ALL WORKERS AT THIS DEPTH FINISH, otherwise math wont add up

        # done with this depth, prepare for next loop
        next_depth = depth - 1
        print("done with depth", depth, "proceeding with depth", next_depth)
        depth = next_depth
        update_increment()

    result = shared_mem_int_list[0]
    print("reached the end, result is", result)
    return result


# start main
master()

# let's be sure
assert sum(input_doc) == shared_mem_int_list[0]
