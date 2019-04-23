#include "tst_markupdocument.h"

void tst_markupdocument::testCase_Html()
{
    MarkupDocument doc;

    QFile file(":doc1.html");
    QVERIFY(file.open(QFile::ReadOnly));

    doc.setContent(file.readAll());

    QVERIFY(doc.isValid());
    QCOMPARE(doc.docType(), DOC_TYPE::HTML);

    MarkupBlock *block = doc.firstBlock();
    QVERIFY(block != Q_NULLPTR);

    if (block)
    {
        QCOMPARE(block->type(), TYPE::DocType);
        QCOMPARE(block->name(), "!doctype");
        QCOMPARE(block->attributes().size(), 0);
        QCOMPARE(block->toString(), "<!doctype html>");
    }

    QList<MarkupBlock*> blocks = doc.blocks();
    QCOMPARE(blocks.size(), 2);
    QCOMPARE(blocks.at(0)->type(), TYPE::DocType);
    QCOMPARE(blocks.at(0)->name(), "!doctype");
    QCOMPARE(blocks.at(0)->attributes().size(), 0);
    QCOMPARE(blocks.at(0)->parentBlock(), Q_NULLPTR);
    QCOMPARE(blocks.at(1)->type(), TYPE::Element);
    QCOMPARE(blocks.at(1)->name(), "html");
    QCOMPARE(blocks.at(1)->attributes().size(), 0);
    QCOMPARE(blocks.at(1)->parentBlock(), Q_NULLPTR);

    // check !doctype
    blocks = doc.blocks().at(0)->children();
    QCOMPARE(blocks.size(), 0);

    // check html
    blocks = doc.blocks().at(1)->children();
    QCOMPARE(blocks.size(), 3);
    QCOMPARE(blocks.at(0)->type(), TYPE::Element);
    QCOMPARE(blocks.at(0)->name(), "head");
    QCOMPARE(blocks.at(0)->attributes().size(), 0);
    QCOMPARE(blocks.at(0)->parentBlock(), doc.blocks().at(1));
    QCOMPARE(blocks.at(1)->type(), TYPE::Element);
    QCOMPARE(blocks.at(1)->name(), "body");
    QCOMPARE(blocks.at(1)->attributes().size(), 1);
    QCOMPARE(blocks.at(1)->attributes()["class"], "l-body");
    QCOMPARE(blocks.at(1)->parentBlock(), doc.blocks().at(1));
    QCOMPARE(blocks.at(2)->type(), TYPE::Element);
    QCOMPARE(blocks.at(2)->name(), "/html");
    QCOMPARE(blocks.at(2)->attributes().size(), 0);
    QCOMPARE(blocks.at(2)->parentBlock(), doc.blocks().at(1));

    // find html
    blocks = doc.findBlocks("html");
    QCOMPARE(blocks.size(), 1);

    // find div
    blocks = doc.findBlocks("div");
    QCOMPARE(blocks.size(), 222);

    // find script
    blocks = doc.findBlocks("script");
    QCOMPARE(blocks.size(), 12);

    // find body
    blocks = doc.findBlocks("body");
    QCOMPARE(blocks.size(), 1);
    // find div in body
    blocks = blocks.at(0)->findBlocks("div");
    QCOMPARE(blocks.size(), 222);
    // find script in body
    blocks = doc.findBlocks("body");
    QCOMPARE(blocks.size(), 1);
    blocks = blocks.at(0)->findBlocks("script");
    QCOMPARE(blocks.size(), 8);

    // find head
    blocks = doc.findBlocks("head");
    QCOMPARE(blocks.size(), 1);
    // find div in head
    blocks = blocks.at(0)->findBlocks("div");
    QCOMPARE(blocks.size(), 0);
    // find script in head
    blocks = doc.findBlocks("head");
    QCOMPARE(blocks.size(), 1);
    blocks = blocks.at(0)->findBlocks("script");
    QCOMPARE(blocks.size(), 4);

    // find a tag with class "c-card-video__link h-flex js-card-link" in body
    blocks = doc.findBlocks("body");
    QCOMPARE(blocks.size(), 1);
    // find a in body with class "c-card-video__link h-flex js-card-link"
    QHash<QString, QVariant> attributes;
    attributes["class"] = "c-card-video__link h-flex js-card-link";
    blocks = blocks.at(0)->findBlocks("a", attributes);
    QCOMPARE(blocks.size(), 25);

    // find a tag with class "c-card-video__link h-flex js-card-link" in div class "c-wall h-flex" id "videos" in body
    blocks = doc.findBlocks("body");
    QCOMPARE(blocks.size(), 1);
    // find div in body with class "c-wall h-flex" id "videos"
    attributes.clear();
    attributes["class"] = "c-wall h-flex";
    attributes["id"] = "videos";
    blocks = blocks.at(0)->findBlocks("div", attributes);
    QCOMPARE(blocks.size(), 1);
    // find a in div class "c-wall h-flex" id "videos" in body
    attributes.clear();
    attributes["class"] = "c-card-video__link h-flex js-card-link";
    blocks = blocks.at(0)->findBlocks("a", attributes);
    QCOMPARE(blocks.size(), 20);
    for (auto block : blocks)
        qWarning() << block->attributes()["href"];
}
