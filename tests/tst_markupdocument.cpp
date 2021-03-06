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
    QCOMPARE(blocks.at(0)->blocks().size(), 0);

    QCOMPARE(blocks.at(1)->type(), TYPE::Element);
    QCOMPARE(blocks.at(1)->name(), "html");
    QCOMPARE(blocks.at(1)->attributes().size(), 0);
    QCOMPARE(blocks.at(1)->parentBlock(), Q_NULLPTR);
    QCOMPARE(blocks.at(1)->blocks().size(), 3);

    // check !doctype
    blocks = doc.blocks().at(0)->blocks();
    QCOMPARE(blocks.size(), 0);

    // check html
    blocks = doc.blocks().at(1)->blocks();
    QCOMPARE(blocks.size(), 3);
    QCOMPARE(blocks.at(0)->type(), TYPE::Element);
    QCOMPARE(blocks.at(0)->name(), "head");
    QCOMPARE(blocks.at(0)->attributes().size(), 0);
    QCOMPARE(blocks.at(0)->parentBlock(), doc.blocks().at(1));
    QCOMPARE(blocks.at(0)->blocks().size(), 22);

    QCOMPARE(blocks.at(1)->type(), TYPE::Element);
    QCOMPARE(blocks.at(1)->name(), "body");
    QCOMPARE(blocks.at(1)->attributes().size(), 1);
    QCOMPARE(blocks.at(1)->attributes()["class"], "l-body");
    QCOMPARE(blocks.at(1)->parentBlock(), doc.blocks().at(1));
    QCOMPARE(blocks.at(1)->blocks().size(), 15);

    QCOMPARE(blocks.at(2)->type(), TYPE::Element);
    QCOMPARE(blocks.at(2)->name(), "/html");
    QCOMPARE(blocks.at(2)->attributes().size(), 0);
    QCOMPARE(blocks.at(2)->parentBlock(), doc.blocks().at(1));
    QCOMPARE(blocks.at(2)->blocks().size(), 0);

    // find html
    blocks = doc.findBlocks("html");
    QCOMPARE(blocks.size(), 1);

    // find div
    blocks = doc.findBlocks("div");
    QCOMPARE(blocks.size(), 222);

    // find script
    blocks = doc.findBlocks("script");
    QCOMPARE(blocks.size(), 12);
    QCOMPARE(blocks.at(0)->blocks().size(), 2);
    QCOMPARE(blocks.at(0)->blocks().at(0)->type(), TYPE::Data);
    QCOMPARE(blocks.at(0)->blocks().at(0)->toString().size(), 15578);

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
        qInfo() << block->attributes()["href"];
}

void tst_markupdocument::testCase_Xml()
{
    MarkupDocument doc;

    QFile file(":doc1.xml");
    QVERIFY(file.open(QFile::ReadOnly));

    doc.setContent(file.readAll());

    QVERIFY(doc.isValid());
    QCOMPARE(doc.docType(), DOC_TYPE::XML);

    MarkupBlock *block = doc.firstBlock();
    QVERIFY(block != Q_NULLPTR);

    if (block)
    {
        QCOMPARE(block->type(), TYPE::DocType);
        QCOMPARE(block->name(), "?xml");
        QCOMPARE(block->attributes().size(), 1);
        QCOMPARE(block->attributes()["version"], "1.0");
        QCOMPARE(block->toString(), "<?xml version='1.0'?>");
        QCOMPARE(block->blocks().size(), 0);
    }

    QList<MarkupBlock*> blocks = doc.blocks();
    QCOMPARE(blocks.size(), 2);
    QCOMPARE(blocks.at(0)->type(), TYPE::DocType);
    QCOMPARE(blocks.at(0)->name(), "?xml");
    QCOMPARE(blocks.at(0)->attributes().size(), 1);
    QCOMPARE(blocks.at(0)->parentBlock(), Q_NULLPTR);
    QCOMPARE(blocks.at(0)->blocks().size(), 0);

    QCOMPARE(blocks.at(1)->type(), TYPE::Element);
    QCOMPARE(blocks.at(1)->name(), "s:Envelope");
    QCOMPARE(blocks.at(1)->attributes().size(), 2);
    QCOMPARE(blocks.at(1)->parentBlock(), Q_NULLPTR);
    QCOMPARE(blocks.at(1)->blocks().size(), 2);
}

void tst_markupdocument::testCase_Html2()
{
    MarkupDocument doc;

    QFile file(":doc2.html");
    QVERIFY(file.open(QFile::ReadOnly));

    doc.setContent(file.readAll());

    QVERIFY(doc.isValid());
    QCOMPARE(doc.docType(), DOC_TYPE::HTML);

    MarkupBlock *block = doc.firstBlock();
    QVERIFY(block != Q_NULLPTR);

    if (block)
    {
        QCOMPARE(block->type(), TYPE::DocType);
        QCOMPARE(block->name(), "!DOCTYPE");
        QCOMPARE(block->attributes().size(), 0);
        QCOMPARE(block->toString(), "<!DOCTYPE html>");
    }

    QList<MarkupBlock*> blocks = doc.blocks();
    QCOMPARE(blocks.size(), 3);
    QCOMPARE(blocks.at(0)->type(), TYPE::DocType);
    QCOMPARE(blocks.at(0)->name(), "!DOCTYPE");
    QCOMPARE(blocks.at(0)->attributes().size(), 0);
    QCOMPARE(blocks.at(0)->parentBlock(), Q_NULLPTR);
    QCOMPARE(blocks.at(0)->blocks().size(), 0);

    QCOMPARE(blocks.at(1)->type(), TYPE::Comment);
    QCOMPARE(blocks.at(1)->name(), "!--");
    QCOMPARE(blocks.at(1)->attributes().size(), 0);
    QCOMPARE(blocks.at(1)->parentBlock(), Q_NULLPTR);
    QCOMPARE(blocks.at(1)->blocks().size(), 0);
    QCOMPARE(blocks.at(1)->toString(), "<!-- saved from url=(0069)https://www.france.tv/france-2/taratata/957911-taratata-100-live.html -->");

    QCOMPARE(blocks.at(2)->type(), TYPE::Element);
    QCOMPARE(blocks.at(2)->name(), "html");
    QCOMPARE(blocks.at(2)->attributes().size(), 0);
    QCOMPARE(blocks.at(2)->parentBlock(), Q_NULLPTR);
    QCOMPARE(blocks.at(2)->blocks().size(), 3);

    // check html
    blocks = doc.blocks().at(2)->blocks();
    QCOMPARE(blocks.size(), 3);
    QCOMPARE(blocks.at(0)->type(), TYPE::Element);
    QCOMPARE(blocks.at(0)->name(), "head");
    QCOMPARE(blocks.at(0)->attributes().size(), 0);
    QCOMPARE(blocks.at(0)->parentBlock(), doc.blocks().at(2));
    QCOMPARE(blocks.at(0)->blocks().size(), 65);

    // check body
    QCOMPARE(blocks.at(1)->type(), TYPE::Element);
    QCOMPARE(blocks.at(1)->name(), "body");
    QCOMPARE(blocks.at(1)->attributes().size(), 1);
    QCOMPARE(blocks.at(1)->parentBlock(), doc.blocks().at(2));
    QCOMPARE(blocks.at(1)->blocks().size(), 24);

    blocks = blocks.at(1)->blocks();
    QCOMPARE(blocks.at(0)->type(), TYPE::Element);
    QCOMPARE(blocks.at(0)->name(), "div");
    QCOMPARE(blocks.at(0)->attributes().size(), 2);
    QCOMPARE(blocks.at(0)->parentBlock(), doc.blocks().at(2)->blocks().at(1));
    QCOMPARE(blocks.at(0)->toString(), "<div id=\"fb-root\" class=\" fb_reset\">");
    QCOMPARE(blocks.at(0)->blocks().size(), 2);
}


void tst_markupdocument::testCase_Html3()
{
    MarkupDocument doc;

    QFile file(":doc3.html");
    QVERIFY(file.open(QFile::ReadOnly));

    doc.setContent(file.readAll());

    QVERIFY(doc.isValid());
    QCOMPARE(doc.docType(), DOC_TYPE::HTML);

    MarkupBlock *block = doc.firstBlock();
    QVERIFY(block != Q_NULLPTR);

    if (block)
    {
        QCOMPARE(block->type(), TYPE::DocType);
        QCOMPARE(block->name(), "!DOCTYPE");
        QCOMPARE(block->attributes().size(), 0);
        QCOMPARE(block->toString(), "<!DOCTYPE html>");
    }

    QList<MarkupBlock*> blocks = doc.blocks();
    QCOMPARE(blocks.size(), 2);
    QCOMPARE(blocks.at(0)->type(), TYPE::DocType);
    QCOMPARE(blocks.at(0)->name(), "!DOCTYPE");
    QCOMPARE(blocks.at(0)->attributes().size(), 0);
    QCOMPARE(blocks.at(0)->parentBlock(), Q_NULLPTR);
    QCOMPARE(blocks.at(0)->blocks().size(), 0);

    // check html
    blocks = doc.blocks().at(1)->blocks();
    QCOMPARE(blocks.size(), 3);
    QCOMPARE(blocks.at(0)->type(), TYPE::Element);
    QCOMPARE(blocks.at(0)->name(), "head");
    QCOMPARE(blocks.at(0)->attributes().size(), 0);
    QCOMPARE(blocks.at(0)->parentBlock(), doc.blocks().at(1));
    QCOMPARE(blocks.at(0)->blocks().size(), 70);

    // check body
    QCOMPARE(blocks.at(1)->type(), TYPE::Element);
    QCOMPARE(blocks.at(1)->name(), "body");
    QCOMPARE(blocks.at(1)->attributes().size(), 4);
    QCOMPARE(blocks.at(1)->parentBlock(), doc.blocks().at(1));
    QCOMPARE(blocks.at(1)->blocks().size(), 13);

    QHash<QString, QVariant> attributes;
    attributes["class"] = "pl-video-title";
    QList<MarkupBlock*> results = blocks.at(1)->findBlocks("td", attributes);
    for (auto tdBlock : results)
    {
        QList<MarkupBlock*> results_aBlocks = tdBlock->findBlocks("a");
        if (results_aBlocks.size() > 1)
        {
            if (results_aBlocks.at(0)->attributes().contains("href"))
            qInfo() << results_aBlocks.at(0)->attributes()["href"];
        }
    }

    QCOMPARE(results.size(), 33);

    attributes["class"] = "pl-header-title";
    results = blocks.at(1)->findBlocks("h1", attributes);
    for (auto h1Block : results)
    {
        if (!h1Block->blocks().empty() && h1Block->blocks().at(0)->type() == TYPE::Data)
            qInfo() << "TITLE" << h1Block->blocks().at(0)->toString();
    }
}

void tst_markupdocument::testCase_Html4()
{
    MarkupDocument doc;

    QFile file(":doc4.html");
    QVERIFY(file.open(QFile::ReadOnly));

    doc.setContent(file.readAll());

    QVERIFY(doc.isValid());
    QCOMPARE(doc.docType(), DOC_TYPE::HTML);

    MarkupBlock *block = doc.firstBlock();
    QVERIFY(block != Q_NULLPTR);

    if (block)
    {
        QCOMPARE(block->type(), TYPE::DocType);
        QCOMPARE(block->name(), "!DOCTYPE");
        QCOMPARE(block->attributes().size(), 0);
        QCOMPARE(block->toString(), "<!DOCTYPE html>");
    }

    QList<MarkupBlock*> blocks = doc.blocks();
    QCOMPARE(blocks.size(), 3);
    QCOMPARE(blocks.at(0)->type(), TYPE::DocType);
    QCOMPARE(blocks.at(0)->name(), "!DOCTYPE");
    QCOMPARE(blocks.at(0)->attributes().size(), 0);
    QCOMPARE(blocks.at(0)->parentBlock(), Q_NULLPTR);
    QCOMPARE(blocks.at(0)->blocks().size(), 0);

    // check comment
    QCOMPARE(blocks.at(1)->type(), TYPE::Comment);
    QCOMPARE(blocks.at(1)->name(), "!--");
    QCOMPARE(blocks.at(1)->attributes().size(), 0);
    QCOMPARE(blocks.at(1)->parentBlock(), Q_NULLPTR);
    QCOMPARE(blocks.at(1)->blocks().size(), 0);

    // check html
    blocks = doc.blocks().at(2)->blocks();
    QCOMPARE(blocks.size(), 4);
    QCOMPARE(blocks.at(0)->type(), TYPE::Element);
    QCOMPARE(blocks.at(0)->name(), "head");
    QCOMPARE(blocks.at(0)->attributes().size(), 0);
    QCOMPARE(blocks.at(0)->parentBlock(), doc.blocks().at(2));
    QCOMPARE(blocks.at(0)->blocks().size(), 37);

    QCOMPARE(blocks.at(1)->type(), TYPE::Element);
    QCOMPARE(blocks.at(1)->name(), "xi:include");
    QCOMPARE(blocks.at(1)->attributes().size(), 1);
    QCOMPARE(blocks.at(1)->parentBlock(), doc.blocks().at(2));
    QCOMPARE(blocks.at(1)->blocks().size(), 0);

    QCOMPARE(blocks.at(2)->type(), TYPE::Element);
    QCOMPARE(blocks.at(2)->name(), "body");
    QCOMPARE(blocks.at(2)->attributes().size(), 0);
    QCOMPARE(blocks.at(2)->parentBlock(), doc.blocks().at(2));
    QCOMPARE(blocks.at(2)->blocks().size(), 0);
    qWarning() << blocks.at(2)->toString();

    QCOMPARE(blocks.at(3)->type(), TYPE::Element);
    QCOMPARE(blocks.at(3)->name(), "/html");
    QCOMPARE(blocks.at(3)->attributes().size(), 0);
    QCOMPARE(blocks.at(3)->parentBlock(), doc.blocks().at(2));
    QCOMPARE(blocks.at(3)->blocks().size(), 0);
}

void tst_markupdocument::testCase_Html_Paw_Patrol()
{
    MarkupDocument doc;

    QFile file(":Vidéos Paw Patrol par type - TF1.htm");
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
    QCOMPARE(blocks.at(0)->blocks().size(), 0);

    QList<MarkupBlock*> aBlocks = doc.findBlocks("a");
    QCOMPARE(aBlocks.size(), 127);
}
