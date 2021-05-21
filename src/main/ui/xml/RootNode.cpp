/*
 * Copyright (C) 2021 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2021 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-plugin-fw
 * Created on: 10 апр. 2021 г.
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

#include <private/ui/xml/RootNode.h>
#include <private/ui/xml/WidgetNode.h>

namespace lsp
{
    namespace ui
    {
        namespace xml
        {
            RootNode::RootNode(UIContext *ctx, const char *name, ctl::Widget *widget): Node(ctx)
            {
                pChild      = NULL;
                pWidget     = widget;
                sName.set_utf8(name);
            }

            RootNode::~RootNode()
            {
                if (pChild != NULL)
                {
                    delete pChild;
                    pChild = NULL;
                }

                pWidget = NULL;

                sName.clear();
            }

            status_t RootNode::start_element(Node **child, const LSPString *name, const LSPString * const *atts)
            {
                // Check that root tag is valid
                if (!name->equals(&sName))
                {
                    lsp_error("expected root element <%s>", sName.get_native());
                    return STATUS_CORRUPTED;
                }

                // Create and initialize widget
                status_t res;
                ctl::Widget *widget     = pWidget;
                if (widget != NULL)
                {
                    // Set widget attributes only
                    if ((res = pContext->set_attributes(widget, atts)) != STATUS_OK)
                        return res;
                }
                else // Instantiate new widget and set it's attributes
                    widget                  = pContext->create_widget(name, atts);

                // No handler?
                if (widget == NULL)
                    return STATUS_OK;

                // Remember the root widget
                pContext->ui()->set_root(widget->widget());

                // Create handler
                pChild = new WidgetNode(pContext, widget);
                if (pChild == NULL)
                    return STATUS_NO_MEM;
                *child  = pChild;

                return STATUS_OK;
            }

        }
    }
}


