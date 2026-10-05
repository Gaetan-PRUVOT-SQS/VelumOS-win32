#ifndef LOGON_TEXT_H
# define LOGON_TEXT_H

# include <stddef.h>
# include <stdint.h>
# include "logon_flow.h"

# define LOGON_PROMPT "Pour commencer, cliquez sur le nom d'utilisateur"
# define LOGON_SHUTDOWN_ASK "Voulez-vous vraiment éteindre l'ordinateur ?"
# define LOGON_MSG_MAX 160

int	logon_message(const t_logonflow *f, uint32_t remaining_s, char *out,
		size_t size);

#endif
