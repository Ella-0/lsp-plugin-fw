/*
 * Copyright (C) 2022 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2022 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-plugin-fw
 * Created on: 24 дек. 2022 г.
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

#include <clap/clap.h>
#include <lsp-plug.in/common/debug.h>
#include <lsp-plug.in/common/singletone.h>
#include <lsp-plug.in/lltl/darray.h>
#include <lsp-plug.in/plug-fw/core/Resources.h>
#include <lsp-plug.in/plug-fw/meta/manifest.h>
#include <lsp-plug.in/plug-fw/plug.h>
#include <lsp-plug.in/stdlib/string.h>

#define CLAP_LOG_FILE           "lsp-lv2.log"

namespace lsp
{
    namespace clap
    {
        //---------------------------------------------------------------------
        // List of plugin descriptors. The list is generated at first demand because
        // of undetermined order of initialization of static objects which may
        // cause undefined behaviour when reading the list of plugin factories
        // which can be not fully initialized.
        static lltl::darray<clap_plugin_descriptor_t> descriptors;
        static meta::package_t *package_manifest = NULL;
        static lsp::singletone_t library;

        //---------------------------------------------------------------------
        // Plugin instance related stuff
        bool CLAP_ABI init(const clap_plugin_t *plugin)
        {
            // TODO
            return true;
        }

        void CLAP_ABI destroy(const clap_plugin_t *plugin)
        {
            // TODO
        }

        bool CLAP_ABI activate(
            const clap_plugin_t *plugin,
            double sample_rate,
            uint32_t min_frames_count,
            uint32_t max_frames_count)
        {
            // TODO
            return true;
        }

        void CLAP_ABI deactivate(const clap_plugin_t *plugin)
        {
        }

        bool CLAP_ABI start_processing(const clap_plugin_t *plugin)
        {
            return true;
        }

        void CLAP_ABI stop_processing(const clap_plugin_t *plugin)
        {
        }

        void CLAP_ABI reset(const clap_plugin_t *plugin)
        {
        }

        clap_process_status CLAP_ABI process(
            const struct clap_plugin *plugin,
            const clap_process_t *process)
        {
            return CLAP_PROCESS_CONTINUE;
        }

        const void * CLAP_ABI get_extension(const clap_plugin_t *plugin, const char *id)
        {
            return NULL;
        }

        void CLAP_ABI on_main_thread(const clap_plugin_t *plugin)
        {
        }

        //---------------------------------------------------------------------
        // Factory-related stuff
        static meta::package_t *load_manifest(resource::ILoader *loader)
        {
            status_t res;
            if (loader == NULL)
                return NULL;

            io::IInStream *is = loader->read_stream(LSP_BUILTIN_PREFIX "manifest.json");
            if (is == NULL)
                return NULL;
            lsp_finally {
                is->close();
                delete is;
            };

            meta::package_t *manifest = NULL;
            if ((res = meta::load_manifest(&manifest, is)) != STATUS_OK)
            {
                lsp_warn("Error loading manifest file, error=%d", int(res));
                return NULL;
            }

            return manifest;
        }

        static ssize_t cmp_descriptors(const clap_plugin_descriptor_t *d1, const clap_plugin_descriptor_t *d2)
        {
            return strcmp(d1->id, d2->id);
        }

        static void gen_descriptors()
        {
            // Check that data already has been initialized
            if (library.initialized())
                return;

            // Obtain the resource loader
            lsp_trace("Obtaining resource loader...");
            resource::ILoader *loader = core::create_resource_loader();
            lsp_finally {
                if (loader != NULL)
                    delete loader;
            };

            // Obtain the manifest
            lsp_trace("Obtaining manifest...");
            meta::package_t *manifest = load_manifest(loader);
            lsp_finally {
                if (manifest != NULL)
                    meta::free_manifest(manifest);
            };
            if (manifest == NULL)
                lsp_trace("No manifest file found");

            // Generate descriptors
            lltl::darray<clap_plugin_descriptor_t> result;
            lsp_finally { result.flush(); };

            lsp_trace("generating descriptors...");

            for (plug::Factory *f = plug::Factory::root(); f != NULL; f = f->next())
            {
                for (size_t i=0; ; ++i)
                {
                    // Skip plugins not compatible with LV2
                    const meta::plugin_t *meta = f->enumerate(i);
                    if ((meta == NULL) || (meta->lv2_uri == NULL))
                        break;

                    // Allocate new descriptor
                    clap_plugin_descriptor_t *d = result.add();
                    if (d == NULL)
                    {
                        lsp_warn("Error allocating LV2 descriptor for plugin %s", meta->lv2_uri);
                        continue;
                    }

                    // Initialize descriptor
                    // TODO
//                    d->URI                  = meta->lv2_uri;
//                    d->instantiate          = instantiate;
//                    d->connect_port         = connect_port;
//                    d->activate             = activate;
//                    d->run                  = run;
//                    d->deactivate           = deactivate;
//                    d->cleanup              = cleanup;
//                    d->extension_data       = extension_data;
                }
            }

            // Sort descriptors
            result.qsort(cmp_descriptors);

        #ifdef LSP_TRACE
            lsp_trace("generated %d descriptors:", int(result.size()));
            for (size_t i=0, n=result.size(); i<n; ++i)
            {
                clap_plugin_descriptor_t *d = result.uget(i);
                lsp_trace("[%4d] %p: %s", int(i), d, d->id);
            }
        #endif /* LSP_TRACE */

            // Commit the generated objects to the global variables
            lsp_singletone_init(library) {
                descriptors.swap(result);
                lsp::swap(package_manifest, manifest);
            };
        }

        static void drop_descriptors()
        {
            lsp_trace("dropping %d descriptors", int(descriptors.size()));
            descriptors.flush();
        }

        static uint32_t get_plugin_count(const clap_plugin_factory_t *factory)
        {
            return descriptors.size();
        }

        static const clap_plugin_descriptor_t *get_plugin_descriptor(
            const clap_plugin_factory_t *factory,
            uint32_t index)
        {
            return descriptors.get(index);
        }

        static const clap_plugin_t *create_plugin(
            const clap_plugin_factory_t *factory,
            const clap_host_t *host,
            const char *plugin_id)
        {
            if (!clap_version_is_compatible(host->clap_version))
                return NULL;

            // Find the corresponding CLAP plugin descriptor
            const clap_plugin_descriptor_t *descriptor = NULL;
            for (size_t i=0, n=descriptors.size(); i<n; ++i)
            {
                const clap_plugin_descriptor_t *d   = descriptors.uget(i);
                if ((d != NULL) && (!strcmp(d->id, plugin_id)))
                {
                    descriptor = d;
                    break;
                }
            }
            if (descriptor == NULL)
            {
                lsp_warn("Invalid CLAP descriptor: %s", plugin_id);
                return NULL;
            }

            // Find the plugin by it's identifier in the list of plugin factories
            for (plug::Factory *f = plug::Factory::root(); f != NULL; f = f->next())
            {
                for (size_t i=0; ; ++i)
                {
                    // Find the corresponding CLAP plugin
                    const meta::plugin_t *meta = f->enumerate(i);
                    if ((meta != NULL) && (!strcmp(meta->clap_uid, plugin_id)))
                    {
                        // Create module
                        plug::Module *plugin = f->create(meta);
                        if (plugin == NULL)
                        {
                            lsp_warn("Failed instantiation of CLAP plugin %s", plugin_id);
                            return NULL;
                        }
                        lsp_finally {
                            if (plugin != NULL)
                                delete plugin;
                        };

                        // TODO: Create the wrapper

                        // Allocate the plugin handle and fill it
                        clap_plugin_t *h   = static_cast<clap_plugin_t *>(malloc(sizeof(clap_plugin_t)));
                        if (h == NULL)
                            return NULL;
                        bzero(h, sizeof(*h));

                        // Fill-in the plugin data and return
                        h->desc             = descriptor;
                        h->plugin_data      = NULL;     // TODO: put pointer to the wrapper
                        h->init             = init;
                        h->destroy          = destroy;
                        h->activate         = activate;
                        h->deactivate       = deactivate;
                        h->start_processing = start_processing;
                        h->stop_processing  = stop_processing;
                        h->reset            = reset;
                        h->process          = process;
                        h->get_extension    = get_extension;
                        h->on_main_thread   = on_main_thread;

                        return h;
                    }
                }
            }

            lsp_warn("Invalid CLAP plugin identifier: %s", plugin_id);
            return NULL;
        }


        static const clap_plugin_factory_t plugin_factory =
        {
           .get_plugin_count        = get_plugin_count,
           .get_plugin_descriptor   = get_plugin_descriptor,
           .create_plugin           = create_plugin,
        };

        //---------------------------------------------------------------------
        // Library-related stuff
        static bool init_library(const char *plugin_path)
        {
            gen_descriptors();
            return true;
        }

        static void destroy_library(void)
        {
            drop_descriptors();
            if (package_manifest != NULL)
            {
                meta::free_manifest(package_manifest);
                package_manifest = NULL;
            }
        }

        const void *get_factory(const char *factory_id)
        {
            if (!strcmp(factory_id, CLAP_PLUGIN_FACTORY_ID))
                return &plugin_factory;
            return NULL;
        }

    } /* namespace clap */
} /* namespace lsp */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    extern CLAP_EXPORT
    const clap_plugin_entry_t clap_entry =
    {
       .clap_version    = CLAP_VERSION_INIT,
       .init            = lsp::clap::init_library,
       .deinit          = lsp::clap::destroy_library,
       .get_factory     = lsp::clap::get_factory
    };

#ifdef __cplusplus
}
#endif /* __cplusplus */

