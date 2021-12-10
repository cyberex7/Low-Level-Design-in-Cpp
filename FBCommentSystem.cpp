#include <iostream>
#include <vector>
using namespace std;

class Comment
{
public:
	Comment(string);
	const vector<Comment>& getComments() const;
	const string& getDescription() const;
	void setDescription(const string &description);
	const string& getId() const;
	const string& getParentId() const;
	void setParentId(const string &parentId);
	const string& getPostId() const;
	void setPostId(const string &postId);
	const string& getUserId() const;
	void setUserId(const string &userId);
	void addComment(Comment&);
	void setNestedDescription(string, string);
	void setNestedDelete(string);

private:
	string id;
	string parentId;
	string userId;
	string postId;
	string description;
	vector<Comment> comments;
	string getUniqueCommentId();
};

class Post
{
public:
	Post();
	const vector<Comment>& getComments() const;
	const string& getId() const;
	void addComment(Comment&);
	void addNestedComment(string, Comment&);
	void editComment(string, string, string);
	void deleteComment(string, string);

private:
	string id;
	vector<Comment> comments;
	string getUniquePostId();
};

class User
{
public:
	User(string);
	void addCommentToPost(Post&, Comment&);
	void replyToComment(Post&, string,  Comment&);
	bool editComment(Post&, string, string, string);
	bool deleteComment(Post&, string, string);
	const string& getId() const;
	const string& getName() const;

private:
	string id;
	string name;
	string getUniqueUserId();
};

Comment::Comment(string description)
{
	this->description = description;
	this->id = getUniqueCommentId();
}

const vector<Comment>& Comment::getComments() const {
	return comments;
}

const string& Comment::getDescription() const {
	return description;
}

void Comment::setDescription(const string &description) {
	this->description = description;
}

void Comment::setNestedDescription(string commentId, string description)
{
	for(Comment& comment: comments)
	{
		if(comment.id == commentId)
		{
			comment.setDescription(description);
			return;
		}
	}
}
void Comment::setNestedDelete(string commentId)
{
	int i = 0;
	for(Comment& comment: comments)
	{
		if(comment.id == commentId)
		{
			comments.erase(comments.begin() + i);
			return;
		}
		i++;
	}
}

const string& Comment::getId() const {
	return id;
}

const string& Comment::getParentId() const {
	return parentId;
}

void Comment::setParentId(const string &parentId) {
	this->parentId = parentId;
}

const string& Comment::getPostId() const {
	return postId;
}

void Comment::setPostId(const string &postId) {
	this->postId = postId;
}

const string& Comment::getUserId() const {
	return userId;
}

void Comment::setUserId(const string &userId) {
	this->userId = userId;
}

string Comment::getUniqueCommentId()
{
	static int commentId = 1;
	return 'c' + to_string(commentId++);
}

void Comment::addComment(Comment& comment)
{
	comments.push_back(comment);
}

Post::Post()
{
	this->id = getUniquePostId();
}

string Post::getUniquePostId()
{
	static int postId = 1;
	return 'p' + std::to_string(postId++);
}

void Post::addComment(Comment& comment)
{
	comments.push_back(comment);
}
void Post::addNestedComment(string commentId, Comment& comment)
{
	for(Comment &comm: comments)
	{
		if(comm.getId() == commentId)
		{
			comm.addComment(comment);
			return;
		}
	}
}

void Post::editComment(string parentId, string commentId, string description)
{
	for(Comment &comment: comments)
	{
		if(comment.getId() == parentId)
		{
			if(parentId == commentId)
				comment.setDescription(description);
			else
				comment.setNestedDescription(commentId, description);
			break;
		}
	}
}
void Post::deleteComment(string parentId, string commentId)
{
	int i = 0, index = -1;
	for(Comment &comment: comments)
	{
		if(comment.getId() == parentId)
		{
			if(parentId == commentId)
			{
				index = i;
				break;
			}
			else
			{
				comment.setNestedDelete(commentId);
				return;
			}
			break;
		}
		i++;
	}
	if(index != -1)
		comments.erase(comments.begin()+ index);
}

const vector<Comment>& Post::getComments() const {
	return comments;
}

const string& Post::getId() const {
	return id;
}

User::User(string name)
{
	this->name = name;
	this->id = getUniqueUserId();
}

string User::getUniqueUserId()
{
	static int userId = 1;
	return 'u' + to_string(userId++);
}

void User::addCommentToPost(Post& post, Comment& comment)
{
	comment.setUserId(this->id);
	comment.setPostId(post.getId());
	comment.setParentId(comment.getId());
	post.addComment(comment);
}
void User::replyToComment(Post& post, string parentId, Comment& comment)
{
	comment.setUserId(this->id);
	comment.setPostId(post.getId());
	comment.setParentId(parentId);
	post.addNestedComment(parentId, comment);
}

bool User::editComment(Post& post, string parentId, string commentId, string description)
{
	for(Comment comment: post.getComments())
	{
		if(comment.getId() == commentId)
		{
			if(comment.getUserId() != this->id)
			{
				cout << "Unauthorized to edit comment!\n";
				return false;
			}
			break;
		}
	}

	post.editComment(parentId, commentId, description);
	return true;
}
bool User::deleteComment(Post& post, string parentId, string commentId)
{
	for(Comment comment: post.getComments())
	{
		if(comment.getId() == commentId)
		{
			if(comment.getUserId() != this->id)
			{
				cout << "Unauthorized to delete comment!\n";
				return false;
			}
			break;
		}
	}

	post.deleteComment(parentId, commentId);
	return true;
}

const string& User::getId() const {
	return id;
}

const string& User::getName() const {
	return name;
}


int main() {
	User u1("L");
	Comment c1("First Comment");
	Comment c2("Second Comment");
	Comment c22("Second nested comment");
	Comment c11("First nested comment");
	Comment c12("First Second nested comment");
	Comment c13("First Third nested comment");

	Post p;
	u1.addCommentToPost(p, c1);
	u1.addCommentToPost(p, c2);
	u1.replyToComment(p, c1.getId(), c11);
	u1.replyToComment(p, c1.getId(), c12);
	u1.replyToComment(p, c1.getId(), c13);
	u1.replyToComment(p, c2.getId(), c22);

	for (Comment c : p.getComments())
	{
		cout << c.getDescription().c_str() << "\n";
		for (Comment cmt : c.getComments())
			cout << "\t" << cmt.getDescription().c_str() << "\n";
	}

	cout << "***********************************************************************" << endl;

	string s12 = "1st 2nd comment";
	string s11 = "1st 1st comment";

	u1.editComment(p, c1.getId(), c11.getId(), s11) ;

	for (Comment c : p.getComments())
	{
		cout << c.getDescription().c_str() << "\n";
		for (Comment cmt : c.getComments())
			cout << "\t" << cmt.getDescription().c_str() << "\n";
	}


	cout << "***********************************************************************" << endl;

	u1.deleteComment(p, c1.getId(), c12.getId()) ;
	for (Comment c : p.getComments())
	{
		cout << c.getDescription().c_str() << "\n";
		for (Comment cmt : c.getComments())
			cout << "\t" << cmt.getDescription().c_str() << "\n";
	}

	cout << "***********************************************************************" << endl;

	u1.deleteComment(p, c1.getId(), c1.getId()) ;
	for (Comment c : p.getComments())
	{
		cout << c.getDescription().c_str() << "\n";
		for (Comment cmt : c.getComments())
			cout << "\t" << cmt.getDescription().c_str() << "\n";
	}
	return 0;
}
