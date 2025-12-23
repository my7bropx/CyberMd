//! HTML Renderer for CyberMD
//!
//! Converts AST to HTML with proper formatting and styling

use cybermd_ast::ASTNode;

pub struct HtmlRenderer {
    buffer: String,
}

impl HtmlRenderer {
    pub fn new() -> Self {
        Self {
            buffer: String::new(),
        }
    }

    pub fn render(&mut self, node: &ASTNode) -> String {
        self.buffer.clear();
        self.render_node(node);
        self.buffer.clone()
    }

    fn render_node(&mut self, node: &ASTNode) {
        match node {
            ASTNode::Document { children, .. } => {
                for child in children {
                    self.render_node(child);
                }
            }

            ASTNode::Heading { level, text, children, .. } => {
                self.buffer.push_str(&format!("<h{}>", level));
                self.buffer.push_str(&escape_html(text));
                // Render inline children if any
                for child in children {
                    self.render_node(child);
                }
                self.buffer.push_str(&format!("</h{}>\n", level));
            }

            ASTNode::Paragraph { text, inline_nodes, .. } => {
                self.buffer.push_str("<p>");
                self.buffer.push_str(&escape_html(text));
                // Render inline nodes
                for child in inline_nodes {
                    self.render_node(child);
                }
                self.buffer.push_str("</p>\n");
            }

            ASTNode::CodeBlock { language, code, .. } => {
                // Special handling for Mermaid diagrams
                if language == "mermaid" {
                    self.buffer.push_str(&format!(
                        "<div class=\"mermaid\">\n{}\n</div>\n",
                        code  // Don't escape for Mermaid
                    ));
                } else {
                    let lang_attr = if !language.is_empty() {
                        format!(" class=\"language-{}\"", escape_html(language))
                    } else {
                        String::new()
                    };
                    self.buffer.push_str(&format!(
                        "<pre><code{}>{}</code></pre>\n",
                        lang_attr,
                        escape_html(code)
                    ));
                }
            }

            ASTNode::InlineCode { text, .. } => {
                self.buffer.push_str(&format!("<code>{}</code>", escape_html(text)));
            }

            ASTNode::Bold { text, .. } => {
                self.buffer.push_str(&format!("<strong>{}</strong>", escape_html(text)));
            }

            ASTNode::Italic { text, .. } => {
                self.buffer.push_str(&format!("<em>{}</em>", escape_html(text)));
            }

            ASTNode::Link { text, url, .. } => {
                self.buffer.push_str(&format!(
                    "<a href=\"{}\">{}</a>",
                    escape_html(url),
                    escape_html(text)
                ));
            }

            ASTNode::List { ordered, items, .. } => {
                let tag = if *ordered { "ol" } else { "ul" };
                self.buffer.push_str(&format!("<{}>\n", tag));
                for item in items {
                    self.render_node(item);
                }
                self.buffer.push_str(&format!("</{}>\n", tag));
            }

            ASTNode::ListItem { text, children, .. } => {
                self.buffer.push_str("<li>");
                self.buffer.push_str(&escape_html(text));
                for child in children {
                    self.render_node(child);
                }
                self.buffer.push_str("</li>\n");
            }

            ASTNode::Blockquote { children, .. } => {
                self.buffer.push_str("<blockquote>\n");
                for child in children {
                    self.render_node(child);
                }
                self.buffer.push_str("</blockquote>\n");
            }

            ASTNode::HorizontalRule { .. } => {
                self.buffer.push_str("<hr>\n");
            }
        }
    }
}

impl Default for HtmlRenderer {
    fn default() -> Self {
        Self::new()
    }
}

fn escape_html(s: &str) -> String {
    s.replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
        .replace('"', "&quot;")
        .replace('\'', "&#39;")
}

#[cfg(test)]
mod tests {
    use super::*;
    use cybermd_ast::ASTNode;

    #[test]
    fn test_render_heading() {
        let mut renderer = HtmlRenderer::new();
        let heading = ASTNode::new_heading(1, "Test".to_string());
        let html = renderer.render(&heading);
        assert!(html.contains("<h1>"));
        assert!(html.contains("Test"));
        assert!(html.contains("</h1>"));
    }

    #[test]
    fn test_render_code_block() {
        let mut renderer = HtmlRenderer::new();
        let code = ASTNode::new_code_block("rust".to_string(), "fn main() {}".to_string());
        let html = renderer.render(&code);
        assert!(html.contains("<pre>"));
        assert!(html.contains("language-rust"));
        assert!(html.contains("fn main()"));
    }

    #[test]
    fn test_escape_html() {
        assert_eq!(escape_html("<script>"), "&lt;script&gt;");
        assert_eq!(escape_html("a & b"), "a &amp; b");
    }
}
