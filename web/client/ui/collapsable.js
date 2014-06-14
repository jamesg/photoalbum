var domjs = require('domjs/lib/html5')(document);

exports.Collapsable = function(title, content) {
    this._documentFragment = domjs.build(
            exports.template.bind(this, title, content)
            );
}

exports.Collapsable.prototype.hideButton = function() {
    this._button().style.display = 'none';
}

exports.Collapsable.prototype.fragment = function() {
    return this._documentFragment;
}

exports.Collapsable.prototype.element = function() {
    return this._div();
}

exports.Collapsable.prototype.setContent = function(content) {
    this._contentDiv().innerHTML = '';
    this._contentDiv(content);
}

/*
 * Template for a collapsable section.  Section has a title and can be
 * collapsed using a pushbutton at the top right.
 *
 * Parameters:
 *  title:   String title to display above the collapsable section.
 *  content: Document fragment to display inside the collapsable section.
 */
exports.template = function(title, content) {
    // Put everything inside a div.
    this._div = div();

    this._div().classList.toggle('collapsable', true);

    // Header and button above the content.
    var _h2 = h2({ style: 'float: left; margin: 0 0 0 0' }, title);
    this._button = button({ class: 'pure-button', style: 'float: right' });

    // Content div.
    this._contentDiv = div({ style: 'clear: both; width: 100%' });
    this._contentDiv().classList.toggle('collapsable-inner', true);
    // Footer (clears the content).
    var footer = div({ style: 'clear: both' });
    footer().classList.toggle('collapsable-footer', true);

    var hidden = false;

    this._button(
        {
            onclick: (function() {
                if(hidden)
                    this._contentDiv().style.display = 'block';
                else
                    this._contentDiv().style.display = 'none';
                hidden = !hidden;
            }).bind(this)
        },
        '+'
      );

    this._contentDiv(content);

    this._div(this._button, _h2, this._contentDiv, footer);
}

