var domjs = require('domjs/lib/html5')(document);

/*!
 * DomJS template for displaying an Open Iconic icon.
 *
 * \param icon Name of the icon to display.  A complete list is available from https://useiconic.com/open/ .
 */
exports.icon = function(icon) {
    return span(
            {
                class: 'oi',
                'data-glyph': icon,
                title: icon,
                'aria-hidden': true
            },
            ' '
        );
    //return svg(
            //{
                //viewbox: '0 0 8 8',
                //class: 'icon'
            //},
            //use(
                //{
                    //'xlink:href': '/open-iconic/sprite/open-iconic.svg#' + icon,
                    //class: 'icon-' + icon
                //}
               //)
            //);
};

