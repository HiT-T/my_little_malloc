name and netID of both partners:
Yuhan Li yl2355
Vishal Nagamalla vn218

descriptions of test programs:

## **Overview**
`memtest.c` tests:
- Correctness of `mymalloc()` and `myfree()`
- Error detection for invalid `free()` calls
- Proper coalescing of free chunks
- Edge cases such as `malloc(0)` and `free(NULL)`

---

## **Test Cases**
### **1. Basic Allocation and Free Test**
- **Purpose**: Checks whether `mymalloc()` properly allocates memory and `myfree()` correctly deallocates it.
- **Procedure**:
  1. Allocate `OBJECTS` small memory chunks.
  2. Fill each chunk with a unique byte pattern.
  3. Verify the memory is correctly assigned.
  4. Free all allocated memory.
- **Expected Behavior**:
  - Each chunk should retain its value.
  - Memory should be correctly freed without corruption.
  - Leak detector should report **no leaks**.

### **2. `mymalloc()` Coalescing Free Chunks (Forward Only)**
- **Purpose**: Tests whether `mymalloc()` correctly merges adjacent free chunks when a large allocation is requested.
- **Procedure**:
  1. Allocate and free `OBJECTS` small chunks sequentially.
  2. Attempt to allocate a large chunk equal to the total size of all freed memory.
- **Expected Behavior**:
  - Allocation should succeed **only if forward coalescing is implemented**.
  - If `mymalloc()` does not coalesce properly, allocation will fail.

### **3. `myfree()` Coalescing Free Chunks**
- **Purpose**: Ensures `myfree()` correctly merges adjacent free chunks.
- **Procedure**:
  1. Allocate `OBJECTS` small memory chunks.
  2. Free them **in reverse order**.
  3. Attempt to allocate a single large chunk.
- **Expected Behavior**:
  - Allocation should succeed **if myfree() coalesces forward chunks**.
  - If `myfree()` does not merge properly, allocation will fail.

### **4. Large Allocation Detection**
- **Purpose**: Ensures `mymalloc()` correctly detects and rejects requests larger than available memory.
- **Procedure**:
  1. Request a block larger than `MEMLENGTH - HEADERSIZE`.
  2. Request a block equal to `MEMLENGTH`.
  3. Request `malloc(0)`.
- **Expected Behavior**:
  - **Large allocations should return NULL** with an error message.
  - **`malloc(0)` should return NULL**.

### **5. Normal Unallocatable Cases**
- **Purpose**: Ensures `mymalloc()` does not allocate when memory is exhausted.
- **Procedure**:
  1. Allocate `OBJECTS + 1` chunks.
  2. Free all `OBJECTS` valid chunks.
  3. Check the last allocation.
- **Expected Behavior**:
  - `malloc()` should fail when requesting memory beyond capacity.
  - **If it succeeds, there's a bug in allocation logic**.

### **6. Invalid `free()` - Out of Heap**
- **Purpose**: Ensures `myfree()` detects and prevents freeing non-heap pointers.
- **Procedure**:
  1. Call `free()` on an **unallocated stack variable**.
  2. Call `free(NULL)`.
  3. Call `free()` on an arbitrary address.
- **Expected Behavior**:
  - **`free(&x)` should terminate with an error**.
  - **`free(NULL)` should be ignored** (no crash).
  - **Freeing an invalid pointer should terminate with an error**.

### **7. Invalid `free()` - Not at Chunk Start**
- **Purpose**: Ensures `myfree()` rejects freeing an address that is not at the start of a chunk.
- **Procedure**:
  1. Allocate a chunk.
  2. Try to `free(ptr + 1)`.
- **Expected Behavior**:
  - `myfree()` should **detect and reject** the invalid free.

### **8. Double Free Detection**
- **Purpose**: Ensures `myfree()` detects double frees.
- **Procedure**:
  1. Allocate a chunk.
  2. Free it twice.
- **Expected Behavior**:
  - The second `free()` should trigger an **error** and terminate.

---

# **mymalloc.c Design Notes**

## **1. Chunk Splitting and Allocation Behavior**
- `mymalloc()` **splits** free chunks when possible.
- If the chunk is **only slightly larger than requested**, it **allocates the entire chunk**.

## **2. Forward-Only Coalescing**
- `myfree()` **only merges free chunks in the forward direction**.
- It **does not merge with previous chunks**.

## **3. Behavior of Edge Cases**
| **Case** | **Behavior** |
|----------|-------------|
| `malloc(0)` | **Returns `NULL`** |
| `free(NULL)` | **Does nothing** |
| `malloc(large_size)` | **Fails with error message** |
| `free(outside_heap_ptr)` | **Fails with error and exits** |
| `free(not_chunk_start_ptr)` | **Fails with error and exits** |
| `double free()` | **Fails with error and exits** |

---