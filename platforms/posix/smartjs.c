#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include <sj_hal.h>
#include <sj_v7_ext.h>
#include <sj_conf.h>
#include <string.h>
#include <sj_i2c_js.h>
#include <sj_spi_js.h>
#include <sj_mongoose.h>
#include <sj_mongoose_ws_client.h>
#include <sj_gpio_js.h>

#include "smartjs.h"

struct v7 *v7;

void init_conf(struct v7 *v7) {
  /* TODO(alashkin): make the filename overridable */
  const char *conf_file_name = "smartjs.conf";
  FILE *fconf = NULL;
  struct stat st;
  char *buf = NULL;

  if ((fconf = fopen(conf_file_name, "r")) == NULL ||
      stat(conf_file_name, &st) == -1) {
    fprintf(stderr, "Cannot open %s\n", conf_file_name);
    return;
  }

  buf = calloc(st.st_size + 1, 1);

  if (fread(buf, 1, st.st_size, fconf) != st.st_size) {
    fprintf(stderr, "Cannot read %s\n", conf_file_name);
    goto cleanup;
  }

  sj_init_conf(v7, buf);

cleanup:
  if (fconf != NULL) {
    fclose(fconf);
  }

  if (buf != NULL) {
    free(buf);
  }
}

void init_smartjs(struct v7 *_v7) {
  v7 = _v7;

  sj_init_v7_ext(v7);
  init_conf(v7);

  mongoose_init();
  sj_init_simple_http_client(v7);
  sj_init_ws_client(v7);

  init_i2cjs(v7);
  init_spijs(v7);
  init_gpiojs(v7);
}
