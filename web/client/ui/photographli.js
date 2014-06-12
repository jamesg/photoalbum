var domjs = require('domjs/lib/html5')(document);

var api  = require('../api');
var util = require('../util');

var ConfirmButton = require('./confirmbutton').ConfirmButton;

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

/*
 * Template for the photograph details form.  Contains title, location,
 * caption, date taken and comma-separated list of tags.
 */
exports.PhotographLi.prototype._formTemplate = function(photograph) {
    var p = div;

    var tags = [];
    for(tagI in photograph.tags)
        tags.push(photograph.tags[tagI].tag);

    var inputs = {
        title:    input({ name: 'title',    value: photograph.title    }),
        location: input({ name: 'location', value: photograph.location }),
        caption:  input({ name: 'caption',  value: photograph.caption  }),
        taken:    input({ name: 'taken',    value: photograph.taken    }),
        tags:     input({ name: 'tags',     value: tags.join(', ')     })
    };

    var messageBox = p({ class: 'messagebox' });

    var _form = form(
            {
                'class': 'editable',
                'onsubmit': function() {
                    var submissionData =
                        util.mapValues(
                            inputs,
                            function(field) {
                                return field().value;
                            }
                            );
                    submissionData.tags = submissionData.tags.split(', ');
                    submissionData.photograph_id = photograph.photograph_id;
                    // Validate submissionData.
                    if(submissionData.title == '')
                    {
                        messageBox().innerHTML = 'Title is required';
                        return false;
                    }
                    if(submissionData.location == '')
                    {
                        messageBox().innerHTML = 'Location is required';
                        return false;
                    }
                    if(submissionData.taken == '')
                    {
                        messageBox().innerHTML = 'Date is required';
                        return false;
                    }

                    api.updatePhotograph(submissionData, function() {});
                    messageBox().innerHTML = 'Photograph updated';
                    return false;
                }
            },
            table(
                tr(td({ colspan: 2 }, messageBox)),

                tr(td('Title'),    td(inputs.title)),
                tr(td('Location'), td(inputs.location)),
                tr(td('Caption'),  td(inputs.caption)),
                tr(td('Taken'),    td(inputs.taken)),
                tr(td('Tags'),     td(inputs.tags)),
                tr(td(),           td(input({ type: 'submit', value: 'Save' })))
                )
            );

    return _form;
}

/*
 * Template for a detailed view of the photograph.  Comprises a large image,
 * details form, list of albums, album selection (to add the photograph to
 * another album) and the delete button.
 */
exports.PhotographLi.prototype._detailTemplate = function(photograph) {
    var p = div;

    var image = img(
            {
                style: 'max-width: 100%',
                src: '/jpeg_image' +
                    util.queryString(
                        {
                            photograph_id: photograph.photograph_id,
                            width: 700,
                            height: 500
                        }
                        ),
                alt: photograph.title,
                onclick: this.toggleDetail.bind(this)
            }
           );

    var paragraph = p(photograph.title);

    // List of albums that this photograph is a member of.
    var albumList = ul();
    albumList().classList.toggle('smallalbumlist', true);

    for(albumI in photograph.albums)
        albumList(
                li(
                    photograph.albums[albumI].name,
                    (new ConfirmButton(
                           'Remove',
                           (function(albumId) {
                               api.removePhotographFromAlbum(
                                   photograph.photograph_id,
                                   albumId,
                                   this._rebuild.bind(this)
                                   );
                           }).bind(this, photograph.albums[albumI].album_id)
                           )
                     ).element()
                    )
                );

    var _form = this._formTemplate(photograph);

    var scaleDiv = div();
    var scales = [
        { 'width': 400,  'height': 300  },
        { 'width': 640,  'height': 480  },
        { 'width': 800,  'height': 600  },
        { 'width': 1024, 'height': 768  },
        { 'width': 1200, 'height': 1024 },
        { 'width': 1440, 'height': 900  }
        ];
    for( i in scales ) {
        scaleDiv( a(
            {
                href: '/jpeg_image' + util.queryString(
                        {
                            photograph_id: photograph.photograph_id,
                            width: scales[i].width,
                            height: scales[i].height
                        }
                        )
            },
            scales[i].width + 'x' + scales[i].height
            ),
            ' '
            );
    }

    scaleDiv( a(
        {
            href: '/jpeg_image_fullsize' + util.queryString(
                      {
                          photograph_id: photograph.photograph_id
                      }
                      )
        },
        'Full size'
        )
        );

    var albumSelect = select();
    api.albumList((function(option, err, albums) {
        for(albumI in albums)
        {
            albumSelect(
                option(
                    { value: albums[albumI].album_id },
                    albums[albumI].name
                    )
                );
        }
    }).bind(this, option)); //< Binding option because it was not defined.  Why?
    var albumForm = form(
            {
                'class': 'editable',
                onsubmit: (function() {
                    try {
                        console.log('add photograph ' + this._photograph.photograph_id + ' ' + albumSelect().value);
                        api.addPhotographToAlbum(
                            this._photograph.photograph_id,
                            albumSelect().value,
                            (function(err) {
                                if(err)
                                    console.log('adding photograph to album: ' + err);
                                this._rebuild();
                            }).bind(this)
                            );
                    } catch (e) {
                        console.log('while adding photograph to album: ' + e);
                    }
                    return false;
                }).bind(this)
            },
            table(
                tr(
                    td(albumSelect),
                    td(input({ type: 'submit', value: 'Add to album' }))
                   )
                )
            );

    this._element(
            {
                'class': 'detailli'
            },
            image,
            paragraph,
            _form,
            albumList,
            albumForm,
            scaleDiv,
            div(
                { 'class': 'editable' },
                (new ConfirmButton(
                    'Delete',
                    (function() {
                        api.deletePhotograph(photograph.photograph_id, function() {});
                    }).bind(this)
                    )
                ).element()
               )
            );
}

/*
 * Template for a photograph appearing in a list.
 */
exports.PhotographLi.prototype._smallTemplate = function(photograph) {
    this._element(
            {
                'class': 'smallli'
            },
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

