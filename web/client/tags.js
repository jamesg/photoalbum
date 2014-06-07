var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var TagList        = require('./ui/taglist').TagList;
var Collapsable       = require('./ui/collapsable').Collapsable;
var PhotographList = require('./ui/photographlist').PhotographList;

exports.Tags = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.Tags.prototype.element = function() {
    return this._element;
}

exports.Tags.prototype._template = function() {
    this._element = div();

    // Create the tag list.
    var tagListCollapsable = new Collapsable('Tags', null);
    this._element(tagListCollapsable.element());
    var tagList = new TagList(
            function(tag) {
                api.photographsWithTag(
                    tag,
                    function(err, list) {
                        photographList.setList(list);
                    }
                    );
            }
            );
    api.tags(
            function(err, tags) {
                tagList.setTagList( util.values(
                        util.mapValues(tags,
                            function(tag) { return tag.tag; }
                            )
                        ) );
            }
            );
    tagListCollapsable.setContent(tagList.element());

    // Create the photograph list.
    var photographListCollapsable = new Collapsable('Photographs', null);
    this._element(photographListCollapsable.element());

    var photographList = new PhotographList(
            function(photograph) {
                console.log('photograph id: ' + photograph.photograph_id);
            }
            );
    photographList.setList([]);
    photographListCollapsable.setContent(photographList.element());
}

