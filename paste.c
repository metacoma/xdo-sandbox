#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <xdo.h>

#define DEBUG
#define WID_MIN_LEN 3

extern int errno;

int window_activate(xdo_t *dpy, Window wid) {
#ifdef DEBUG
  printf("Activte window: %lu\n", wid);
#endif // DEBUG

  assert(dpy != NULL);

  int ret = 0;

  ret = xdo_activate_window(dpy, wid);

  if (ret) {
    return ret;
  }

  xdo_wait_for_window_active(dpy, wid, 1);

  return 0;
}

int main(int argc, char **argv) {
  int ret;
  xdo_t *dpy;
  Window window;
  char *window_id;

  if (argc != 2) {
    fprintf(stderr, "Usage, %s <window_id>\n", argv[0]);
    return EXIT_FAILURE;
  }

  window_id = argv[1];

  if (window_id && strlen(window_id) < WID_MIN_LEN) {
    fprintf(stderr, "Usage, %s 0x0000000\n", argv[0]);
    return EXIT_FAILURE;
  }
#ifdef DEBUG
  printf("Calculate window id for %s\n", window_id);
#endif


  window = strtol(window_id, NULL, 16);

  if (errno) {
    fprintf(stderr, "Fail convert window_id '%s' to long int: %s\n", window_id, strerror(errno));
    return EXIT_FAILURE;
  }

  dpy = xdo_new(NULL);

  assert(dpy != NULL);


  ret = window_activate(dpy, window);

  if (ret) {
    fprintf(stderr, "Window activate fail, window_id '%s', ret: %d\n", window_id, ret);
    return EXIT_FAILURE;

  }

  return EXIT_SUCCESS;
}
