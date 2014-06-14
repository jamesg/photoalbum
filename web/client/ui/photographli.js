var domjs = require('domjs/lib/html5')(document);

var api  = require('../api');
var util = require('../util');

var AlbumsForm = require('./albumsform').AlbumsForm;
var ConfirmButton = require('./confirmbutton').ConfirmButton;
var PhotographDetailsForm = require('./photographdetailsform').PhotographDetailsForm;
var PhotographView = require('./photographview').PhotographView;

/*
 * Displays a photograph in a list item.  The list item can be expanded to give
 * a more detailed view of the photograph and a form to change photograph
 * details.
 *
 * photograph is an object of the form:
 * {
 *  'photograph_id': id,
 *  'title': title,
 *  'caption': caption,
 *  'taken': date taken,
 *  'location': location
 * }
 */
exports.PhotographLi = function(photograph) {
    this._documentFragment = domjs.build(this._template.bind(this));

    this._detail = false;
    this._photograph = photograph;
    this._rebuild();
}

exports.PhotographLi.prototype._rebuild = function() {
    if(this._detail)
    {
        api.photograph(
            this._photograph.photograph_id,
            (function(err, photograph) {
                this._element().innerHTML = '';
                if(err)
                    console.log(err);
                else
                    domjs.build(
                            this._detailTemplate.bind(this, photograph)
                            );
            }).bind(this)
            );
    }
    else
        domjs.build(
                this._smallTemplate.bind(this, this._photograph)
                );
}

exports.PhotographLi.prototype.element = function() {
    return this._element;
}

exports.PhotographLi.prototype.toggleDetail = function() {
    this._detail = !this._detail;
    this._element().innerHTML = '';
    this._rebuild();
}

/*
 * Default template.  Provide an element that can be inserted into a list.
 */
exports.PhotographLi.prototype._template = function() {
    this._element = li();
}

exports.PhotographLi.prototype._scalesTemplate = function(photographId) {
    var scales = [
        { width: 400, height: 300 },
        { width: 640, height: 480 },
        { width: 800, height: 600 },
        { width: 1024, height: 768 },
        { width: 1200, height: 1000 },
        { width: 1440, height: 900 }
    ];
    var _ul = ul();
    for(scale in scales) {
        var params = scales[scale];
        params.photograph_id = photographId;
        _ul(li(a(
            {
                href: '/jpeg_image' + util.queryString(params)
            },
            params.width, 'x', params.height
            )));
    }
    return div({ class: 'pure-menu pure-menu-open' }, _ul);
}

/*
 * Template for a detailed view of the photograph.  Comprises a large image,
 * details form, list of albums, album selection (to add the photograph to
 * another album) and the delete button.
 */
exports.PhotographLi.prototype._detailTemplate = function(photograph) {
    var albumsForm = new AlbumsForm(photograph.photograph_id);
    var photographView = new PhotographView(
        photograph.photograph_id,
        function() {}
        );
    var photographDetailsForm = new PhotographDetailsForm(
        photograph
        );
    var closeButton = button(
            { class: 'pure-button', onclick: this.toggleDetail.bind(this) },
            'Close'
            );
    this._element(
        { class: 'detailli' },
        div(
            { class: 'pure-g' },
            div({ class: 'pure-u-20-24', style: 'text-align: center' }, photographView.element()),
            div({ class: 'pure-u-4-24', style: 'text-align: right' }, closeButton),
            div({ class: 'pure-u-8-24' }, photographDetailsForm.element()),
            div({ class: 'pure-u-8-24' }, albumsForm.element()),
            div({ class: 'pure-u-8-24' }, this._scalesTemplate(photograph.photograph_id))
           )
        );
}

/*
 * Template for a photograph appearing in a list.
 */
exports.PhotographLi.prototype._smallTemplate = function(photograph) {
    this._element(
            { class: 'smallli' },
            img(
                {
                    src: '/jpeg_image' +
                        util.queryString(
                            {
                                photograph_id: photograph.photograph_id,
                                width: 120,
                                height: 120
                            }
                            ),
                    alt: 'Photograph',
                    //alt: photograph.title,
                    onclick: this.toggleDetail.bind(this)
                }
               )
            );
}

