# Bitwise Operations in C

## Introduction
The goal of this repository is to acquaint you with bitwise operations, explaining what they are, how they work, and what they can be used for.

## Chapter 1: It's All Binary
In C (and most other high-level languages), our variables have _types_. These types are indicative of a few things. Of course, a variable of type `int` will store an integer value, but the key to understanding these bitwise operations is to know that under the hood, all types are stored in memory (anywhere, stack, heap, wherever) as binary. Here's an example of what happens when you store a simple integer value on the stack in C:

```c
int main(int argc, char** argv) {
    int x = 2;
    return 0;
}
```

After compiling to assembly, the code might look like this (I'm using ARM assembly here, and I've annotated the code using comments):
```asm
.section .text
.global main

main:
    ; Set up the stack for the function
    stp x29, x30 [sp, -16]! ; Save previous frame pointer & link register
    mov x29, sp ; Setup a new frame pointer
    
    ; Initialize x with 2
    ; IN C: int x = 2;
    mov w0, 2 ; Move 2 into the w0 register
    str w0, [sp, 16] ; Store the contents of w0 (2) at a 16-byte offset from the stack pointer
    ; Essentially, the above line stores 2 on the stack.
    
    mov w0, 0 ; Move 0 into w0, prepare for return
    
    ; Clear stack
    ldp x29, x30, [sp], 32 ; Restore frame pointer and link register
    ret ; Return
```

Note that most compilers would _not_ actually store a variable like the one I showed on the stack, as it is unused. However, if it is used multiple times, it would be stored on the stack something like the above.

If we looked at the location where our variable was stored on the stack (while it is there, of course), we would see something like:

| Memory Address | Value Stored (Hex) | Value Stored (Binary) |
|----------------|--------------------|-----------------------|
| `0x1000`       | `0x02`             | `00000010`            |
| `0x1001`       | `0x00`             | `00000000`            |
| `0x1002`       | `0x00`             | `00000000`            |
| `0x1003`       | `0x00`             | `00000000`            |

This assumes that your system is little-endian. I won't go into endianness here, but you can read more about it [here](https://developer.mozilla.org/en-US/docs/Glossary/Endianness).

The key thing I'd like you to notice about the table above is that even though our integer is only 2 bits long, it takes up 4 bytes (32 bits) of memory. Now, don't freak out—this is normal and expected. One of the many things that C and your compiler do is set standards for the types you invoke. So when I create an `int` variable, the compiler knows to allocate 4 bytes (again, 32 bits) of memory. We can even test this using the `sizeof()` operator in C. 

> #### The `sizeof()` Operator
> `sizeof()` is not an actual C function. Instead, at compile time, the compiler replaces the expression with the size of the given data type. You can even use this with your own types, like typedefs and/or structs:
>
> ```c
> #include <stdio.h> 
> 
> typedef struct {
>   char name[64];
>   int age;
> } Person;
> 
> int main(int argc, char** argv) {
>   printf("A Person is %lu bytes long.\n", sizeof(Person));
>   return 0;
> }
> ```

One other thing you might be asking is how negative numbers are stored. Excellent question. Numbers can be _signed_ or _unsigned_, but by default, they're signed. If an integer is signed, it sacrifices its most significant bit to be the "sign bit." If the sign bit is 1, the number is negative; otherwise it's positive. An astute reader might realize that the change that happens here is in the range of possible numbers. Consider 8-bit numbers. There are 256 possible numbers to represent (given by 2^8). With an unsigned 8-bit integer, the values 0–255 can be represented; with a signed 8-bit int, -128–127 can be represented.

To get the inverse of a binary number, use two's compliment. Let's find -5 in binary.

1. Start with 5. In binary, 5 is `0101`. The leading 0 is the sign.
2. Invert each bit. `0101` &rarr; `1010`.
3. Add 1 to this number (ignoring any possible overflow). `1010 + 0001 = 1011`.

#### Your Turn!
1. Confirm that -5 is `1011` in binary by performing two's compliment on it to get 5, or `0101`.
2. Write a C program that prints the size of an int in both bytes and bits. Use the code above as a starting point. *Hint: to convert from bytes to bits, how many bits are in a byte?*
3. Fill in the following table with sizes of different types, modifying your program to check them.

| Type                                         | Size (bytes) | Size (bits) |
|----------------------------------------------|--------------|-------------|
| int                                          |              |             |
| int64_t                                      |              |             |
| int8_t                                       |              |             |
| char                                         |              |             |
| bool (you'll need to `#include <stdbool.h>`) |              |             |
| long int                                     |              |             |
| short int                                    |              |             |
| long long int                                |              |             |
| double                                       |              |             |
| double                                       |              |             |

<details>
    <summary>Question 1 Response</summary>
    
1. Start with -5: `1011`.
2. Invert each bit: `1011` &rarr; `0100`.
3. Add 1: `0100 + 0001 = 0101`

</details>


<details>
    <summary>Question 2 Sample Response</summary>
 Here's an example of what your simple program might look like (you can also check it out at `Chapter 1/SizeOfOperatorTest.c`).

 ```c
 #include <stdio.h>

 int main(int argc, char** argv) {
    printf("The size of an int is %lu bytes, or %lu bits.\n", sizeof(int), sizeof(int) * 8);
    return 0;
 }
```

Go ahead and compile it using `gcc` and check out its output:

```zsh
cd Chapter\ 1
gcc -o sizeof SizeOfOperatorTest.c
./sizeof
```
</details>

<details>
    <summary>Question 3 Response</summary>

| Type                                         | Size (bytes) | Size (bits) |
|----------------------------------------------|--------------|-------------|
| int                                          | 4            | 32          |
| int64_t                                      | 8            | 64          |
| int8_t                                       | 1            | 8           |
| char                                         | 1            | 8           |
| bool (you'll need to `#include <stdbool.h>`) | 1            | 8           |
| long int                                     | 4            | 32          |
| short int                                    | 2            | 16          |
| long long int                                | 8            | 64          |
| double                                       | 4            | 32          |
| double                                       | 8            | 64          |

</details>

> ### Take _This_ Home
> 
> The main point I'd like you to keep in mind is that **with control of every bit, we can optimize our memory usage**. Though that has little effect on modern systems, in the case of embedded computing, every byte counts. **By manually reading and writing bits as opposed to typical variable values, we can harness more functionality from less storage.**

## Chapter 2: Operating on Bits

Now that we've covered data types and how data is stored, we're ready to introduce the idea of bitwise operations. Put simply, a **bitwise operation** is an operation done on each bit of a piece of data. Let's take a look at each bitwise operator. Then, we'll implement them in C.

### And (&)

Written `x & y` in C. This operator takes the corresponding bits of each number and performs an and operation. An and operation returns 1 (true) _if and only if both bits are 1_. This means that two bits that are both 0 do **not** return 1—they return 0. The result is the number made up of the binary string of results from each and. It's easiest to see this in a truth table. 

Consider the operation `int result = 3 & 5`. First, convert 3 and 5 to binary.
Now, we have `int result = 011 & 101`. Consider the following truth table:

| Bit A | Bit B | AND |
|-------|-------|-----|
| 0     | 1     | 0   |
| 1     | 0     | 0   |
| 1     | 1     | 1   |

The result of the and operation is `001`, which when converted to decimal is 1.

### Or (|)

Written `x | y` in C. This operator takes the corresponding bits of each number and performs an or operation. An or operation returns 1 _if either bit is 1_. As with other bitwise operators, the result is the number made up of the binary string of results from each or.

Consider the operation `int result = 3 | 5`. First, convert 3 and 5 to binary.
Now, we have `int result = 011 | 101`. Consider the following truth table:

| Bit A | Bit B | OR |
|-------|-------|----|
| 0     | 1     | 1  |
| 1     | 0     | 1  |
| 1     | 1     | 1  |

The result of the or operation is `111`, which when converted to decimal is 7.

### Xor (^)

Written `x ^ y` in C. This operator takes the corresponding bits of each number and performs an xor (exclusive or) operation. An xor operation returns 1 _if and only if one of the two bits is 1_. As with other bitwise operators, the result is the number made up of the binary string of results from each or.

Consider the operation `int result = 3 ^ 5`. First, convert 3 and 5 to binary.
Now, we have `int result = 011 ^ 101`. Consider the following truth table:

| Bit A | Bit B | XOR |
|-------|-------|-----|
| 0     | 1     | 1   |
| 1     | 0     | 1   |
| 1     | 1     | 0   |

The result of the xor operation is `110`, which when converted to decimal is 6.

### Left Shift (<<)

Written `x << amount` Unlike the above operators, this operator only operates on one number. Each bit in the given number is shifted to the left by the given amount. Any bits that reach the end of the number are truncated (and zeros appear on the right side). Let's walk through an example.

Consider `int result = 5 << 2`. As you know, 5 is `101` in binary. Let's walk through each iteration of the shift.

**Initial**

| 1 | 0 | 1 |
|---|---|---|

**After one shift**

| 0 | 1 | 0 |
|---|---|---|

**Result**

| 1 | 0 | 0 |
|---|---|---|

The binary result is `100`, which is 4 in decimal.

### Right Shift (>>)

Written `x >> amount` This operator is just like the left shift, except it works in the opposite direction. Each bit in the given number is shifted to the right by the given amount. Any bits that reach the end of the number are truncated (and zeros appear on the left side). Let's walk through an example.

Consider `int result = 3 >> 2`. As you know, 3 is `011` in binary. Let's walk through each iteration of the shift.

**Initial**

| 0 | 1 | 1 |
|---|---|---|

**After one shift**

| 0 | 0 | 1 |
|---|---|---|

**Result**

| 0 | 0 | 0 |
|---|---|---|

The binary result is `000`, which is 0 in decimal.

### Not (~)
Written `~x`. The not operator inverts all the bits of the given number. Once again, we'll use a truth table to elaborate.

Consider `int result = ~5`. As you know, 5 is `101` in binary.

| Bit A | ~ Bit A |
|-------|---------|
| 1     | 0       |
| 0     | 1       |
| 1     | 0       |

Hence, the result of the not operation is `010`, which is 2 in binary.

> #### Left Shift & Right Shift Restrictions
> There are a few notable restrictions placed on these shift operations. For starters, you cannot shift bits a negative number of times—that just doesn't make sense! Also, shifting for more than the number of bits allocated to your variable is considered _undefined_. You *can* do it, but its output is not guaranteed to be constant for a given value. Finally, although not a restriction per-say, shifting 0 times simply doesn't perform a shift.


### Your Turn!
1. Complete a truth table for each of the following. Consider all values to be unsigned. Convert to decimal when complete.
- `8 & 2`
- `6 | 3`
- `7 ^ 5`
- `(5 & 2) & (4 & 3)`
- `(5 | 2) & (4 & 3)`
- `(5 & 2) ^ (4 | 3)`

2. Complete each operation. Consider all values to be unsigned and as long as the longest value in the problem needs to be (i.e., if you have the largest value of 8, deal with 4 bits). Convert to decimal when complete.
- `~6`
- `9 << 4` (here consider the value to be of length 32, so there's room to left shift).
- `~(7 & 8)`
- `(2 | 6) >> 1`
- `8 >> (~2)`
- `~((3 >> 2) ^ ~7) & (~(7 >> 4) | 2)`

<details>
    <summary>Question 1 Response</summary>

- `8 & 2` &rarr; `1000 & 0010`

| Bit A | Bit B | AND |
|-------|-------|-----|
| 1     | 0     | 0   |
| 0     | 0     | 0   |
| 0     | 1     | 0   |
| 0     | 0     | 0   |

&rArr; `0000`, which is 0 in decimal.

- `6 | 3` &rarr; `110 | 011`

| Bit A | Bit B | OR |
|-------|-------|----|
| 1     | 0     | 1  |
| 1     | 1     | 1  |
| 0     | 1     | 1  |

&rArr; `111`, which is 7 in decimal.

- `7 ^ 5` &rarr; `111 ^ 101`

| Bit A | Bit B | XOR |
|-------|-------|-----|
| 1     | 1     | 0   |
| 1     | 0     | 1   |
| 1     | 1     | 0   |

&rArr; `010`, which is 2 in decimal.


- `(5 & 2) & (4 & 3)` &rarr; `(101 & 010) & (100 & 011)`

| Bit A | Bit B | A AND B | Bit C | Bit D | C AND D | (A AND B) AND (C AND D) |
|-------|-------|---------|-------|-------|---------|-------------------------|
| 1     | 0     | 0       | 1     | 0     | 0       | 0                       |
| 0     | 1     | 0       | 0     | 1     | 0       | 0                       |
| 1     | 0     | 0       | 0     | 1     | 0       | 0                       |

&rArr; `000`, which is 0 in decimal.

- `(5 | 2) & (4 & 3)` &rarr; `(101 | 010) & (100 & 011)`

| Bit A | Bit B | A OR B | Bit C | Bit D | C AND D | (A OR B) AND (C AND D) |
|-------|-------|--------|-------|-------|---------|------------------------|
| 1     | 0     | 1      | 1     | 0     | 0       | 0                      |
| 0     | 1     | 1      | 0     | 1     | 0       | 0                      |
| 1     | 0     | 1      | 0     | 1     | 0       | 0                      |

&rArr; `000`, which is 0 in decimal.

- `(5 & 2) ^ (4 | 3)` &rarr; `(101 & 010) ^ (100 | 011)`

| Bit A | Bit B | A AND B | Bit C | Bit D | C OR D | (A AND B) XOR (C OR D) |
|-------|-------|---------|-------|-------|--------|------------------------|
| 1     | 0     | 0       | 1     | 0     | 1      | 1                      |
| 0     | 1     | 0       | 0     | 1     | 1      | 1                      |
| 1     | 0     | 0       | 0     | 1     | 1      | 1                      |

&rArr; `111`, which is 7 in decimal.

</details>

<details>
    <summary>Question 2 Response</summary>

- `~6` &rarr; `~110` &rArr; `011`, which is 3 in decimal.

- `9 << 4` &rarr; `001001 << 4` &rArr; `100100`, which is 36 in decimal.

- `~(7 & 8)` &rarr; `~(0111 & 1000)` &rarr; `~(0000)` &rArr; `1111`, which is 15 in decimal.

- `(2 | 6) >> 1` &rarr; `(010 | 110) >> 1` &rarr; `110 >> 1` &rArr; `011`, which is 3 in decimal.

- `8 >> (~2)` &rarr; `1000 >> ~(10)` &rarr; `1000 >> (01)` &rarr; `1000 >> 1` &rArr; `0100`, which is 4 in decimal.

- `~((3 >> 2) ^ ~7) & (~(7 >> 4) | 2)`

To solve this, I suggest splitting into halves:

`~((3 >> 2) ^ ~7)` and `(~(7 >> 4) | 2)`

`~((3 >> 2) ^ ~7)` &rarr; `~((011 >> 2) ^ ~(111))` &rarr; `~((000) ^ ~(111))` &rarr; `~(000 ^ 000)` &rarr; `111`
`(~(7 >> 4) | 2)` &rarr; `(~(111 >> 4) | 010)` &rarr; `(~(000) | 010)` &rarr; `(111 | 010)` &rarr; `111`

Hence, `111 & 111` &rArr; `111`, which is 7 in decimal.

</details>

## Chapter 3: Applying Bitwise Operations in C

This chapter is all about writing C code that utilizes bitwise operators. Before we get to doing bitwise operations, we should begin by writing a function that can write the binary equivalent of a given variable.

To do this, we use a mask. We initialize it to contain a `1` in the most significant (leftmost in little-endian systems) bit followed by zeros. With each iteration of a loop, we right shift the mask by 1, moving the 1 all the way "across" the mask. When we use the `&` operator on the pointer and the mask, any non-zero value means that a `1` occurred somewhere in the result. Since there's only one `1` in the mask, we know exactly where this occurred. Since the loop moves from left to right, we can just append the corresponding bit's character to the string. The string is one character longer than the size because it needs to contain the null character (`\0`). This is how `printf` knows to stop printing, and omitting it can lead to numerous errors and/or unexpected behaviors (like the printing of other data from in memory).

```c
void printBinary(unsigned int decimal) {

    // To determine size (in bits), we multiply the maximum size of an unsigned int by 8 (to convert from bytes to bits).
    int size = sizeof(decimal) * 8;

    // This counts the leading zeros of the number, then subtracts them from the size.
    // Hence, we start at the first bit set to 1 (unless decimal == 0)
    size -= __builtin_clz(decimal);

    if(size == 0) size = 1; // Handle zero case, we need one space to display "0."

    int curr = 0;
    char binary[size + 1];
    for(unsigned int mask = 1 << (size - 1); mask != 0; mask >>= 1) {
        if((decimal & mask) != 0) {
            binary[curr] = '1';
        } else {
            binary[curr] = '0';
        }
        curr++;
    }

    binary[curr] = '\0';

    printf("%s", binary);

}
```

> ### Bitwise Assignment Operators
> All bitwise operators, except for not (`~`), can be used in the assignment fashion. This means you can add an equals sign right next to one of the bitwise operator. For example, in
> 
> ```c
> int a = 2;
> a &= 7;
> ```
>
> `a` is both the first operand and the destination. In other words, the value of a & 7 is stored in a. This works for all bitwise operators aside from the not (`~`) operator.

Now, I'd like to provide a few case study like prompts for you to try. Sample responses are available.

### Case Study 1: Unix File Permissions

One use case of bitwise operations is in the Unix permission system. You've probably run the command

```zsh
chmod 777 some-file
```

But what do each of the numbers mean? And why 7? The reality is, binary is at play here, and 7 should tip you off. Recall that 7 in binary is `111`. The number being passed here is acting as three flags or booleans glued together.

The three digits specified are for three classes of users:
- The file owner;
- Group members of the file owner;
- and everyone else.

As I mentioned above, each digit is a conglomeration of three flags, each representing a permission. The flag (binary bit) in the fours place represents read permission, the twos place is for write permission, and the ones is for execute. So, ```chmod 777 some-file``` is doing this under the hood:


#### File Permissions: ``some-file``

| Group         | Read | Write | Execute | Decimal |
|---------------|------|-------|---------|---------|
| Owner         | 1    | 1     | 1       | 7       |
| Owner's Group | 1    | 1     | 1       | 7       |
| Everyone Else | 1    | 1     | 1       | 7       |

In other words, all permissions are given to all.

#### Task

Design a simple permissions checker that takes in a full file permission value (a three-digit number) and checks for a given set of specific permissions (i.e., owner write, everyone execute, etc.). For an example, check the `Chapter 3` folder.

<details>
    <summary>Stuck? Grab a hint.</summary>

After taking in a full number, you'll need to convert it to an `int` (from a `char*`). Then, use modular arithmetic to break down each permission set. Remember, the first digit represents the owner's permissions, the second is for the owner's user group, and the third is for everyone.

To check if a specific permission occurs in a permission set, bitwise and the given permission with the set.
</details>

### Case 2: Subnetting a Network

If you've ever configured a router, you may have noticed an option to configure the "subnet mask." Usually, this is set to `255.255.255.0`, but why? Firstly, we need to remember that each byte of an IPv4 address is separated by a '.'. When dealing with the type of network you're most familiar with (a class C network), there are 3 bytes dedicated to the network and the final byte is for the host. 

Being that the subnet mask is a mask, you might be catching on to its purpose. For each bit you "borrow" from the host byte, two subnets are created.

> #### Network Address
> The **network address** has all _host_ bits set to `0`. This means any bit surrendered to create
> a subnet still could be set to `1`. 

> #### Read More!
> Learn more about subnets by checking out [this](https://networklessons.com/subnetting/subnetting-in-binary) website.

#### Task

In C, write a program that takes in an IPv4 address and a subnet mask and finds and outputs the network address that the IPv4 address lives in. For an example, check the `Chapter 3` folder.

<details>
    <summary>Stuck? Grab a hint.</summary>

You'll need to store each byte of the address and mask as a numerical value. To find the network address, consider which (bitwise) operation between the mask and address will create the intended effect.

</details>

## Closing
I hope this explainer was useful for you! I wrote it because I wanted to learn about bitwise operations myself. I've checked it, but some things could be wrong, so feel free to correct me via a pull request. I'm so happy to have been able to provide this resource for you!


## About Me

Hi! I'm Jackson, a student of computer science & French at Lafayette College and an aspiring researcher and professor in computer science. I'm currently interested in the fields of bioinformatics and low-level programming/systems. To learn more about me, check out my [site](https://jacksoneshbaugh.github.io).
