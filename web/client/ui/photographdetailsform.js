var domjs = require('domjs/lib/html5')(document);

var api  = require('../api');
var util = require('../util');

exports.PhotographDetailsForm = function(photograph) {
    this._documentFragment = domjs.build(this._template.bind(this, photograph));
}

exports.PhotographDetailsForm.prototype.element = function() {
    return this._element;
}

/*
 * Template for the photograph details form.  Contains title, location,
 * caption, date taken and comma-separated list of tags.
 */
exports.PhotographDetailsForm.prototype._template = function(photograph) {
    var tags = [];
    for(tagI in photograph.tags)
        tags.push(photograph.tags[tagI].tag);

    var inputs = {
        title:    input({ name: 'title',    value: photograph.title    }),
        location: input({ name: 'location', value: photograph.location }),
        caption:  input({ name: 'caption',  value: photograph.caption  }),
        taken:    input({ name: 'taken',    value: photograph.taken    }),
        tags:     input({
            name: 'tags',
            placeholder: 'Comma-separated tags',
            value: tags.join(', ')
            })
    };

    var inlineInput = function(input_, label_ ) {
        return div(
                { class: 'pure-control-group' },
                label({ for: input_().name }, label_),
                input_
                );
    }

    var _form = form(
            {
                'class': 'pure-form pure-form-stacked',
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
                    return false;
                }
            },
            div(
                    inlineInput(inputs.title, 'Title'),
                    inlineInput(inputs.location, 'Location'),
                    inlineInput(inputs.caption, 'Caption'),
                    inlineInput(inputs.taken, 'Taken'),
                    inlineInput(inputs.tags, 'Tags'),
                    inlineInput(input({ type: 'submit', value: 'Save' }), '')
                    )
            );

    this._element = _form;
    return _form;
}

