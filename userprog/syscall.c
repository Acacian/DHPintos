#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/loader.h"
#include "userprog/gdt.h"
#include "threads/flags.h"
#include "intrinsic.h"

void syscall_entry (void);
void syscall_handler (struct intr_frame *);

/* System call.
 *
 * Previously system call services was handled by the interrupt handler
 * (e.g. int 0x80 in linux). However, in x86-64, the manufacturer supplies
 * efficient path for requesting the system call, the `syscall` instruction.
 *
 * The syscall instruction works by reading the values from the the Model
 * Specific Register (MSR). For the details, see the manual. */

#define MSR_STAR 0xc0000081         /* Segment selector msr */
#define MSR_LSTAR 0xc0000082        /* Long mode SYSCALL target */
#define MSR_SYSCALL_MASK 0xc0000084 /* Mask for the eflags */

void
syscall_init (void) {
	write_msr(MSR_STAR, ((uint64_t)SEL_UCSEG - 0x10) << 48  |
			((uint64_t)SEL_KCSEG) << 32);
	write_msr(MSR_LSTAR, (uint64_t) syscall_entry);

	/* The interrupt service rountine should not serve any interrupts
	 * until the syscall_entry swaps the userland stack to the kernel
	 * mode stack. Therefore, we masked the FLAG_FL. */
	write_msr(MSR_SYSCALL_MASK,
			FLAG_IF | FLAG_TF | FLAG_DF | FLAG_IOPL | FLAG_AC | FLAG_NT);
}

/*
struct gp_registers {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;   can use 4
	uint64_t r9;    can use 5
	uint64_t r8;    can use 6
	uint64_t rsi;   can use 2
	uint64_t rdi;   can use 1
	uint64_t rbp;
	uint64_t rdx;   can use 3
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
} __attribute__((packed));
*/

/* The main system call interface */
//! Project 2 - System calls
void
syscall_handler (struct intr_frame *f UNUSED) {
	// TODO: Your implementation goes here.

  // printf ("system call!\n");
  // thread_exit ();

  //? 시스템콜 호출 번호 - %rax
  //? 인자 - %rdi, $rsi, %rdx, %r10, %r8, %r9

  int sys_number = f->R.rax;

  switch (sys_number) {

    case SYS_HALT:          /* Halt the operating system. */
      halt ();
      break;

    case SYS_EXIT:          /* Terminate this process. */
      exit (f->R.rdi);
      break;

    case SYS_FORK:          /* Clone current process. */
      fork (f->R.rdi);
      break;

    case SYS_EXEC:
      printf("Performing syscall 3\n");
      // syscall 3 처리
      break;

    case SYS_WAIT:
      printf("Performing syscall 4\n");
      // syscall 4 처리
      break;

    case SYS_CREATE:
      printf("Performing syscall 5\n");
      // syscall 5 처리
      break;

    case SYS_REMOVE:
      printf("Performing syscall 6\n");
      // syscall 6 처리
      break;
    case SYS_OPEN:
      printf("Performing syscall 7\n");
      // syscall 7 처리
      break;

    case SYS_FILESIZE:
      printf("Performing syscall 8\n");
      // syscall 8 처리
      break;

    case SYS_READ:
      printf("Performing syscall 9\n");
      // syscall 9 처리
      break;

    case SYS_WRITE:
      // printf("Performing syscall 10\n");
      printf("%s", f->R.rsi);
      // syscall 10 처리
      break;

    case SYS_SEEK:
      printf("Performing syscall 11\n");
      // syscall 11 처리
      break;

    case SYS_TELL:
      printf("Performing syscall 12\n");
      // syscall 12 처리
      break;

    case SYS_CLOSE:
      printf("Performing syscall 13\n");
      // syscall 13 처리
      break;

    default:
      printf ("system call!\n");
      thread_exit ();
      // printf("Unknown syscall number\n");
      // break;
  }
}

/*
	SYS_EXEC,                    3 Switch current process.
	SYS_WAIT,                    4 Wait for a child process to die.
	SYS_CREATE,                  5 Create a file.
	SYS_REMOVE,                  6 Delete a file.
	SYS_OPEN,                    7 Open a file.
	SYS_FILESIZE,                8 Obtain a file's size.
	SYS_READ,                    9  Read from a file.
	SYS_WRITE,                   10 Write to a file.
	SYS_SEEK,                    11 Change position in a file.
	SYS_TELL,                    12 Report current position in a file.
	SYS_CLOSE,                   13 Close a file.
};
*/
void 
halt (void) {

  power_off ();
}

void 
exit (int status) {
  struct thread *curr = thread_current ();
  curr->exit_status = status;
  
  thread_exit ();
}

tid_t 
fork(const char *thread_name) {
  /* 
    %RBX, %RSP, %RBP와 %R12 - %R15 레지스터 복제해야 함
    생성된 자식 프로세스의 pid 반환
    부모 프로세스는 자식 프로세스가 성공적으로 복제되었는지 여부를 알 때까지 fork에서 반환해서는 안 됩니다. 
    즉, 자식 프로세스가 리소스를 복제하지 못하면 부모의 fork() 호출이 TID_ERROR를 반환할 것입니다.
    템플릿은 threads/mmu.c의 pml4_for_each를 사용하여 해당되는 페이지 테이블 구조를 포함한 전체 사용자 메모리 공간을 복사하지만, 
    전달된 pte_for_each_func의 누락된 부분을 채워야 합니다.
    (가상 주소) 참조).
  */
  // int pid = thread_create ();


  // return pid;
}

// int exec (const char *file);
// int wait (pid_t);
// bool create (const char *file, unsigned initial_size);
// bool remove (const char *file);
// int open (const char *file);
// int filesize (int fd);
// int read (int fd, void *buffer, unsigned length);
// int write (int fd, const void *buffer, unsigned length);
// void seek (int fd, unsigned position);
// unsigned tell (int fd);
// void close (int fd);