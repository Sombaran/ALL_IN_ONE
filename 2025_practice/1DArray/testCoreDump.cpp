
/*
 * Set custom path: sudo sysctl -w kernel.core_pattern=/tmp/core.%e.%p
 * Debugging the Dump: gdb ./myprogram /tmp/core.myprogram.1234
 * Use below command
 * sudo sysctl -w kernel.core_pattern=core.%p
 * 
 * 
 * GDB Commands:
 * Perfect — let’s walk through the exact, disciplined workflow you’d use with GDB to root‑cause a production crash from a core dump. Think of this as a checklist you can apply every time:

🛠️ Step‑by‑Step Workflow
1. Collect the artifacts
- Binary: The exact executable that crashed (same build, same symbols).
- Core dump: The core.<pid> file or retrieved via coredumpctl dump.
- Debug symbols: Ideally, compile with -g and avoid stripping symbols. If stripped in production, keep a symbolized build separately.

2. Launch GDB with binary + core
gdb ./your_program core.<pid>


This loads both the executable and the captured process state.

3. Identify the crash point
- Run:
(gdb) bt
- → Backtrace of all threads. Shows where the crash occurred.
- If multiple threads:
(gdb) thread apply all bt
- → Backtrace for every thread.

4. Inspect the crashing frame
- Switch to the top frame:
(gdb) frame 0
- Print local variables:
(gdb) info locals
- Print arguments:
(gdb) info args



5. Check registers and signal
- See which signal killed the process:
(gdb) info signal
- Inspect registers:
(gdb) info registers
- Often you’ll see SIGSEGV with a bad memory address in rip/eip.

6. Inspect memory
- If a pointer looks suspicious:
(gdb) x/16x <address>
- → Hex dump of memory around that address.
- Or dereference:
(gdb) p *ptr
7. Navigate the stack- Move up/down:
(gdb) up
(gdb) down
- Inspect each frame’s locals/args to see how the bad state propagated.
8. Cross‑check with source- If you have debug symbols:
(gdb) list
- → Shows source lines around the crash.
- If not, rely on disassembly:
(gdb) disassemble
9. Common root causes you’ll detect- Null pointer dereference → p *ptr shows invalid memory.
- Use‑after‑free → pointer looks valid but memory contents corrupted.
- Stack overflow → backtrace shows deep recursion.
- Race condition → multiple threads with conflicting states.
10. Document the findings- Save backtrace:
set logging on
bt full
thread apply all bt full
set logging off
- This gives you a reproducible crash report to share with your team.
🚀 Example sessiongdb ./server core.12345
(gdb) bt
#0  0x00007f2a4c123456 in process_request (req=0x0) at server.cpp:42
(gdb) frame 0
(gdb) info locals
req = 0x0
(gdb) p *req
Cannot access memory at address 0x0
Root cause: Null pointer dereference in process_request().👉 The key is: start with the backtrace, then drill down into locals, args, and memory until you see the invalid state.Would you like me to also show you a diagnostic checklist for multi‑threaded crashes (deadlocks, races, etc.), since those are trickier than single‑thread SIGSEGVs?
 * */


#include <csignal>
#include <iostream>

int main() {
    std::raise(SIGABRT); // More explicit than abort()
}


