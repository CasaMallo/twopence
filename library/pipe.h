/*
 * Generic functions implementing the twopence protocol
 * for serial, virtio etc.
 *
 * Copyright (C) 2014-2015 SUSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef PIPE_H
#define PIPE_H

#include "protocol.h"
#include "transaction.h"
#include "connection.h"

/* Base class for all targets using the twopence pipe protocol */
struct twopence_pipe_target {
  struct twopence_target base;

  const struct twopence_pipe_ops *link_ops;

  /* Timeout for keepalives. Set to 0 to disable; -1 to use the default settings */
  int				keepalive;

  /* This holds the fd of the serial port/the socket or whatever else we use to
   * communicate with the server. */
  twopence_conn_t *		connection;

  twopence_protocol_state_t	ps;

  /* "foreground" transaction. This is the transaction that gets
   * cancelled when twopence_interrupt() is called. */
  twopence_transaction_t *	current_transaction;
};


struct twopence_pipe_ops {
  twopence_sock_t *		(*open)(struct twopence_pipe_target *);
};

extern void	twopence_pipe_target_init(struct twopence_pipe_target *, int plugin_type, const struct twopence_plugin *,
			const struct twopence_pipe_ops *);

extern int	twopence_pipe_set_option(struct twopence_target *target, int option, const void *value_p);
extern int	twopence_pipe_run_test(struct twopence_target *, twopence_command_t *, twopence_status_t *);
extern int	twopence_pipe_wait(struct twopence_target *, int, twopence_status_t *);
extern int	twopence_pipe_chat_send(twopence_target_t *opaque_handle, int xid, twopence_iostream_t *stream);
extern int	twopence_pipe_chat_recv(twopence_target_t *opaque_handle, int xid, const struct timeval *deadline);
extern int	twopence_pipe_inject_file (struct twopence_target *, twopence_file_xfer_t *, twopence_status_t *);
extern int	twopence_pipe_extract_file (struct twopence_target *, twopence_file_xfer_t *, twopence_status_t *);
extern int	twopence_pipe_interrupt_command(struct twopence_target *);
extern int	twopence_pipe_exit_remote(struct twopence_target *);
extern void	twopence_pipe_end(struct twopence_target *);

#endif /* PIPE_H */
