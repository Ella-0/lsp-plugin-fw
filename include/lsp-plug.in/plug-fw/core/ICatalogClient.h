/*
 * Copyright (C) 2024 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2024 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-plugin-fw
 * Created on: 18 июн. 2024 г.
 *
 * lsp-plugin-fw is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-plugin-fw is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-plugin-fw. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LSP_PLUG_IN_PLUG_FW_CORE_ICATALOGCLIENT_H_
#define LSP_PLUG_IN_PLUG_FW_CORE_ICATALOGCLIENT_H_

#include <lsp-plug.in/plug-fw/version.h>
#include <lsp-plug.in/common/atomic.h>
#include <lsp-plug.in/dsp-units/shared/Catalog.h>
#include <lsp-plug.in/ipc/Thread.h>
#include <lsp-plug.in/ipc/Mutex.h>
#include <lsp-plug.in/lltl/parray.h>
#include <lsp-plug.in/plug-fw/core/Catalog.h>

namespace lsp
{
    namespace core
    {
        class Catalog;

        /**
         * Catalog client interface
         */
        class ICatalogClient
        {
            private:
                friend class Catalog;

            private:
                Catalog                *pCatalog;
                volatile uint32_t       nRequest;
                volatile uint32_t       nResponse;

            private:
                status_t                do_close();

            public:
                ICatalogClient();
                virtual ~ICatalogClient();

            public:
                /**
                 * Connect to the catalog
                 * @param catalog catalog to connect to
                 * @return status of operation
                 */
                status_t                connect(Catalog *catalog);

                /**
                 * Disconnect from catalog
                 * @return status of operation
                 */
                status_t                close();

                /**
                 * Request the catalog to call update() in it's thread.
                 */
                void                    request();

                /**
                 * Check that client is connected to catalog
                 * @return true if client is connected to catalog
                 */
                bool                    connected() const;

            public:
                /**
                 * Notify client about changes in the catalog and force the client to
                 * update configuration.
                 */
                virtual void            update();

                /**
                 * Apply changes introduced by the client to the catalog.
                 */
                virtual void            apply();
        };

    } /* namespace core */
} /* namespace lsp */




#endif /* LSP_PLUG_IN_PLUG_FW_CORE_ICATALOGCLIENT_H_ */