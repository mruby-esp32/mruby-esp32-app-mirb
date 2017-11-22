#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "rom/uart.h"

#undef getchar
#define getchar uart_getchar

#define STATE_NML 0x00
#define STATE_ESC 0x01
#define STATE_CSI 0x02

_sig_func_ptr
signal(int sig, _sig_func_ptr func)
{
  return SIG_ERR;
}

static void
echo_rewrite(const uint8_t *s, size_t n, int cursor) {
  printf("\e[3G\e[J");
  printf("%.*s", n, s);
  if (n && s[n-1] != '\n') {
    printf("\e[%dG", 3 + cursor);
  }
  fflush(stdout);
}

static int
uart_getchar(void)
{
  static size_t n = 0;
  static uint8_t buf[1024];
  static uint8_t last = 0;
  uint8_t c, s = STATE_NML;
  int v = 0, cursor = 0;

  if (n) {
    c = buf[0];
    memmove(buf, buf + 1, --n);
    return (int)c;
  }
  do {
    c = uart_rx_one_char_block();
    if (last == '\r' && c == '\n') {
      continue;
    }
    last = c;
    if (c == '\b' || c == 0x7f) {
      if (n && cursor) {
        memmove(buf + (cursor - 1), buf + cursor, n - cursor);
        --n, --cursor;
        echo_rewrite(buf, n, cursor);
      }
      continue;
    }
    if (c == '\e') {
      s = STATE_ESC;
      continue;
    }
    if (c == '\r') {
      c = '\n';
    }
    switch (s) {
    case STATE_ESC:
      if (c == '[') {
        s = STATE_CSI;
        v = 0;
      } else {
        s = STATE_NML;
      }
      continue;
    case STATE_CSI:
      switch (c) {
      case '0'...'9':
        v = (v * 10) + (c - '0');
      case ';':
        v = 0;
      case 'A'...'z':
        switch (c) {
        case 'C':
          if (!v) {
            v = 1;
          }
          if (n - cursor < v) {
            v = n - cursor;
          }
          cursor += v;
          echo_rewrite(buf, n, cursor);
          break;
        case 'D':
          if (!v) {
            v = 1;
          }
          if (cursor < v) {
            v = cursor;
          }
          cursor -= v;
          echo_rewrite(buf, n, cursor);
          break;
        }
      default:
        s = STATE_NML;
      }
      continue;
    }
    if (c != '\n') {
      memmove(buf + (cursor + 1), buf + cursor, n - cursor);
      buf[cursor] = c;
      ++n, ++cursor;
    } else {
      buf[n++] = c;
      cursor = n;
    }
    echo_rewrite(buf, n, cursor);
  } while (c != '\n');
  return uart_getchar();
}

#define main mirb_main
#include "../components/mruby/mruby/mrbgems/mruby-bin-mirb/tools/mirb/mirb.c"
#undef main

static void
mirb_task(void *pvParameter)
{
  char *argv[] = {"mirb", NULL};

  mirb_main(1, argv);

  esp_restart();
}

void
app_main()
{
  xTaskCreate(&mirb_task, "mirb_task", 16384, NULL, 5, NULL);
}
