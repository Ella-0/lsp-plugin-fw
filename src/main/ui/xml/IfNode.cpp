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

#include <private/ui/xml/IfNode.h>
#include <private/ui/xml/NodeFactory.h>

namespace lsp
{
    namespace ui
    {
        namespace xml
        {
            //-----------------------------------------------------------------
            NODE_FACTORY_IMPL_START(IfNode)
                if (!name->equals_ascii("ui:if"))
                    return STATUS_NOT_FOUND;

                *child = new IfNode(context, parent);
                return (*child != NULL) ? STATUS_OK : STATUS_NO_MEM;
            NODE_FACTORY_IMPL_END(IfNode)

            //-----------------------------------------------------------------
            IfNode::IfNode(UIContext *ctx, Node *parent): Node(ctx, parent)
            {
                pContext    = ctx;
                bPass       = true;
            }

            IfNode::~IfNode()
            {
                pContext    = NULL;
            }

            status_t IfNode::init(const LSPString * const *atts)
            {
                status_t res;
                bool valid = false;

                for ( ; *atts != NULL; atts += 2)
                {
                    const LSPString *name   = atts[0];
                    const LSPString *value  = atts[1];

                    if ((name == NULL) || (value == NULL))
                        continue;

                    if (name->equals_ascii("test"))
                    {
                        if ((res = pContext->eval_bool(&bPass, value)) != STATUS_OK)
                        {
                            lsp_error("Could not evaluate expression attribute '%s': %s", name->get_native(), value->get_native());
                            return res;
                        }
                        valid = true;
                    }
                    else
                    {
                        lsp_error("Unknown attribute: %s", name->get_utf8());
                        return STATUS_CORRUPTED;
                    }
                }

                if (!valid)
                {
                    lsp_error("Not all attributes are set");
                    return STATUS_CORRUPTED;
                }

                return STATUS_OK;
            }

            status_t IfNode::start_element(Node **child, const LSPString *name, const LSPString * const *atts)
            {
                return (bPass) ? pParent->start_element(child, name, atts) : STATUS_OK;
            }

            status_t IfNode::end_element(const LSPString *name)
            {
                return (bPass) ? pParent->end_element(name) : STATUS_OK;
            }

            status_t IfNode::completed(Node *child)
            {
                return (bPass) ? pParent->completed(child) : STATUS_OK;
            }

            status_t IfNode::leave()
            {
                return (bPass) ? pParent->leave() : STATUS_OK;
            }

            status_t IfNode::enter()
            {
                return (bPass) ? pParent->enter() : STATUS_OK;
            }

        }
    }
}



