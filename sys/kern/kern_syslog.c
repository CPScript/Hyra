/*
 * Copyright (c) 2023-2024 Ian Marco Moffett and the Osmora Team.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Hyra nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/syslog.h>
#include <sys/machdep.h>
#include <sys/tty.h>
#include <dev/vcons/vcons.h>
#include <string.h>

struct vcons_screen g_syslog_screen = {0};

static void
syslog_write(const char *s, size_t len)
{
    size_t tmp_len = len;
    const char *tmp_s = s;

    while (tmp_len--) {
#if defined(__SERIAL_DEBUG)
        serial_dbgch(*tmp_s);
#endif  /* defined(__SERIAL_DEBUG) */
        tty_putc(&g_root_tty, *tmp_s++, TTY_SOURCE_RAW);
    }

    tty_flush(&g_root_tty);
}

void
vkprintf(const char *fmt, va_list *ap)
{
    char buffer[1024] = {0};

    vsnprintf(buffer, sizeof(buffer), fmt, *ap);
    syslog_write(buffer, strlen(buffer));
}

void
kprintf(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vkprintf(fmt, &ap);
    va_end(ap);
}

void
syslog_init(void)
{
    g_syslog_screen.bg = 0x000000;
    g_syslog_screen.fg = 0x808080;

    vcons_attach(&g_syslog_screen);
}
